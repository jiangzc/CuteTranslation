# CuteTranslation
## 简介
CuteTranslation 是Linux系统上基于X11的一款取词翻译软件。帮助用户减少英文阅读障碍。


功能：
1. 取词翻译   
	<img src="pic/demo1.gif" width="600" height="300">  
2. 截图翻译  
	<img src="pic/demo2.gif" width="400" height="274">
3. 悬浮搜索框  
	<img src="pic/demo3.png" width="400" height="305">

特点：
+ Google翻译插件风格
+ html css js 渲染界面
+ 对接百度翻译
+ 百度AI：OCR识别图片内容并翻译
+ 个性化的配置
+ 快捷键 悬浮窗口

## 使用
正在开发中，预计2020年1月-2月正式发布。
## 编译
为 Debian 系的发行版安装开发库
```bash
# Qt, WebEngine, Xlib
sudo apt install qt5-default libxtst-dev libxcb-util0-dev g++  make
sudo apt install libqt5x11extras5 libqt5webengine5 libqt5webenginewidgets5 qtwebengine5-dev
# JS
sudo apt install nodejs
# Python3
pip3 install PyExecJS
pip3 install requests
```
qmake 编译
```bash
mkdir build && cd build
qmake ..
make
```
安装
```
# 安装到build目录
bash install.sh build

# 安装到 /opt/CuteTranslation
sudo bash install.sh
sudo chmod 777 /opt/CuteTranslation
```
运行
```
./CuteTranslation
```
## 捐赠
<img src="pic/alipay.png" width="250" height="250"><img src="pic/wechat.png" width="250" height="250">

## 感谢
+ [words-picker](https://github.com/ziqiangxu/words-picker)
+ [ScreenTranslation](https://github.com/ziqiangxu/words-picker)
+ [BaiduTranslate](https://github.com/ZCY01/BaiduTranslate)