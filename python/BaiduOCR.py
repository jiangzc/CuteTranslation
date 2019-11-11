#!/usr/bin/env python3
# 百度AI 通用文字识别客户端  调用量限制 50000次/天免费 不保证并发
# Demo: https://ai.baidu.com/tech/ocr/general
# API Documents: https://ai.baidu.com/docs#/OCR-API/top
# Authorization: https://ai.baidu.com/docs#/Auth/top
# token是json数据，包含access_token，一个月过期

import os
import sys
import time
import json
import base64
import requests
import signal


token_url = "http://67.216.199.87:5000/token"
access_token = None

def get_token_from_url():
    global access_token
    res = requests.get(token_url, timeout=5)
    with open('token', 'w') as f:
        f.write(res.text)
    access_token = json.loads(res.text)['access_token']


def check_local_token():
    global access_token
    if not os.path.exists('./token'):
        get_token_from_url()
    else:
        with open('token', 'r') as f:
            token = json.loads(f.read())
            if int(time.time()) > int(token['expires_at']):
                # token 过期
                get_token_from_url()
            else:
                access_token = token['access_token']


def recognize_image(picture_path): # picture_path 是绝对路径
    image_string = None
    if not os.path.exists(picture_path):
        print("picture path do not exist")
        return "path do not exist"
    with open(picture_path, 'rb') as f:
        image_string = base64.b64encode(f.read())
    data = {'image': image_string}
    res = requests.post("https://aip.baidubce.com/rest/2.0/ocr/v1/general_basic?access_token=" + 
        access_token, data, timeout=5)
    result_string = ""
    for item in json.loads(res.text)["words_result"]:
        result_string += item["words"] + " "
    print(result_string)
    return result_string


def main():
    os.chdir(os.path.dirname(os.path.abspath(__file__)))
    check_local_token()
    #os.system("gnome-screenshot -a -f /tmp/ocr > /dev/null 2>&1")
    recognize_image("/tmp/ocr")

if __name__ == '__main__':
    main()

