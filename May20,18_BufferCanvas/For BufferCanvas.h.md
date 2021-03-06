# For BufferCanvas.h

头文件包括 BufferCanvas.h、Rect.h 和 Mono.h。适用于辅助制作 ASCII 字符的 C++ 控制台游戏，借助这些头文件可以制作大多数类型的控制台 2D 游戏。

**最近更新为 May20,2018**

1. BufferCanvas.h
   - 双缓冲显示核心，提取控制台显示句柄并生成新控制台句柄作为前置缓冲。
   - 与 Mono.h 联动，需要两个 Mono 对象作为屏幕显示的缓冲区。
   - 可以控制是否显示控制台光标。
   - 新建 BufferCanvas 对象后需要调用 BufferCanvas::init() 初始化。
2. Rect.h
   - 数据成员包含构成控制台屏幕上的矩形区域需要的左上角点坐标及宽高信息。
   - 约定面对屏幕时所见的屏幕左上角为原点，x 轴正半轴向下、y 轴正半轴向右。
   - 可以判断某点是否在当前 Rect 对象表示的矩形内。
   - 可以改变当前 Rect 对象的位置。
3. Mono.h
   - Mono 类继承自 Rect 类，增加了与矩形区域绑定的图案 (表达为 char**)、Mono 对象是否在活跃的标记等。、
   - 向 Mono 对象导入图案信息的方式多样，可以额外设置或者从文本文件导入。推荐使用后一种。
   - Mono 对象既可以作为屏幕显示区域使用，也可以作为屏幕中的物件使用，一个 Mono 对象可以加载到另一个 Mono 对象上。
   - 可以使用 space 在内的字符填充整个 Mono 对象的图案，也可以在 Mono 对象的指定位置写上文字。