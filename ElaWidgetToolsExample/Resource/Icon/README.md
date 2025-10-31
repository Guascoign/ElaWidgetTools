# 应用程序图标说明

## 图标文件位置
请将您的应用程序图标文件命名为 `app.ico` 并放置在以下位置：
`Resource/Icon/app.ico`

## 图标规格要求
- 格式：ICO 格式
- 推荐尺寸：16x16, 32x32, 48x48, 256x256 像素
- 颜色深度：32位（支持透明度）

## 图标创建方法
1. 使用在线工具将 PNG 图片转换为 ICO 格式
2. 使用 GIMP、Photoshop 等图像编辑软件
3. 使用在线ICO生成器如：https://www.favicon-generator.org/

## 如果没有图标文件
如果您暂时没有图标文件，可以：
1. 注释掉 app.rc 中的图标行
2. 或者使用一个默认的图标文件

当前配置会查找：Resource/Icon/app.ico
