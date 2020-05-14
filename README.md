# 跳伞游戏😊
## 安装与配置cocos2d-x
### 下载cocos2d-x
https://www.cocos.com/cocos2dx
下载V4.0版本

### 配置环境
```bash
cd cocos2d-x-4.0
python2 ./setup.py
```
看到以下提示代表安装成功
```
Please execute command: "source /Users/name/.bash_profile" to make added system variables take effect 
```

## 创建一个新游戏
```bash
cocos new HelloWorld -l cpp -d NEW_PROJECTS_DIR 
HelloWorld是你项目的名称
-l cpp 是使用的编程语言
-d NEW_PROJECTS_DIR是保存项目的到NEW_PROJECTS_DIR
```
```
cd HelloWorld
mkdir build
cd build
cocos run --proj-dir .. -p mac
```
生成```mac-build```文件夹
双击打开```HelloWorld.xcodeproj```项目

## 运行我的项目
将```Classes```与```Resource```内的文件进行替换
并在Xcode中添加对应文件即可