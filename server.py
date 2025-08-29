from pywinauto import Application as WinApp
from flask import Flask, Response
import requests, re
import time
import win32gui

flask_app = Flask(__name__)

def get_code(raw_text): 
    import requests
    import re
    
    API_URL = "https://api.perplexity.ai/chat/completions" 
    API_KEY = "pplx-api_key"
    
    extraction_prompt = (
        "Extract the main coding question or request from the following text. "
        "Remove any garbage, noise, or irrelevant content. "
        "Return only the clean, clear coding question or task."
    )
    
    extraction_payload = {
        "model": "sonar",
        "messages": [
            {"role": "system", "content": extraction_prompt},
            {"role": "user", "content": raw_text}
        ],
        "temperature": 0.0,
        "max_tokens": 500,
        "top_p": 1.0
    }
    
    headers = {"Authorization": f"Bearer {API_KEY}", "Content-Type": "application/json"}
    
    try:
        resp = requests.post(API_URL, headers=headers, json=extraction_payload, timeout=60)
        resp.raise_for_status()
        data = resp.json()
        clean_question = data["choices"][0]["message"]["content"].strip()
        
        code_system_prompt = (
            "You are a code-only assistant. Respond ONLY with valid source code suitable for direct use in C++ or python whichever one is better. "
            "Do NOT output any explanations, prose, commentary, headings, bullet points, or Markdown (no fences). "
            "Output exactly and only the code characters to be executed or saved — nothing else. Make sure to add no comments"
        )
        
        code_payload = {
            "model": "sonar-reasoning-pro",
            "messages": [
                {"role": "system", "content": code_system_prompt},
                {"role": "user", "content": clean_question}
            ],
            "temperature": 0.0,
            "max_tokens": 1500,
            "top_p": 1.0
        }
        
        resp = requests.post(API_URL, headers=headers, json=code_payload, timeout=60)
        resp.raise_for_status()
        data = resp.json()
        text = data["choices"][0]["message"]["content"]
        
        START = re.compile(r"^\s*(?:def|class|import|from|package|public|func|#include|<\?php|var|\w+\s*=|return|[{[(]|</)", re.M)
        m = START.search(text)
        return text[m.start():].strip() if m else text.strip()
    except Exception as e:
        print(f"Error in get_code: {e}")
        return None


def get_question():
    hwnd = win32gui.GetForegroundWindow()
    win_title = win32gui.GetWindowText(hwnd)
    print(hwnd)
    print(win_title)
    if "Chrome" in win_title:
        ui_app = WinApp(backend="uia").connect(handle=hwnd)
        dlg = ui_app.window(handle=hwnd)
        document = dlg.child_window(control_type="Document")
        body = ' '.join([e.window_text() for e in document.descendants() if e.window_text().strip()])
    else:
        body = "Active window is not Chrome"
    return body[:5000]

time.sleep(3)

@flask_app.route("/text")
def text():
    question = get_question()
    code = get_code(question)
    return Response(code, mimetype="text/plain")
if __name__ == "__main__":
    flask_app.run(host="0.0.0.0", port=5000, debug=True, use_reloader=False)