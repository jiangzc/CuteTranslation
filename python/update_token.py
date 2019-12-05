#!/usr/bin/env python3
import requests
import os

os.chdir(os.path.dirname(os.path.abspath(__file__)))
token_url = "http://67.216.199.87:5000/token"
res = requests.get(token_url, timeout=5)
print(res.text)
with open('token', 'w') as f:
    f.write(res.text)
