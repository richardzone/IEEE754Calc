说明：

程序使用Qt SDK 4.8.1开发，使用Visual Studio 2010 x86的编译器，其中.pro的是工程配置文件，可以使用Qt Creator或任意文本编辑器直接打开，也可以通过Qt的Visual Studio Addin转换成VS2010的工程项目，但仍需要Qt类库才能正常使用。

编译方法：在Visual Studio 2010 命令提示符环境下，运行qmake即可

另外几个文件分别是：
main.cpp是主程序，用来进入消息事件循环(Event Loop)

widget.h是项目的头文件，其中#include <QtGui>引用了Qt的GUI类库

widget.ui是XML格式的ui界面文件，可以直接修改，也可以使用Qt Designer（Qt设计师）来可视化修改，qmake在编译时会自动调用uic来编译该ui文件，生成widget.cpp中包含的"ui_widget.h"

widget.cpp是程序主要实现文件

Qt采用了信号和槽的机制，本程序主要用到了每当QLineEdit控件在其文字内容被修改时发出的textEdited信号，并自动调用calc和reverseCalc两个函数来进行计算。

程序界面上，QLineEdit采用了inputMask和Validator两种机制来保证用户输入的合法和有效性，并且使用户在更改下面的两个二进制编码数据时，自动加上下划线来引导输入，并实现了浮点数<=>单、双精度三向转换。

程序还有很多缺点不足，比如没有处理denormalized values（gradual underflow）的情况，这个是通过限制浮点数输入框长度，使用户无法输入太小的数字来暂时弥补的，另外+0和-0暂未处理，期待以后能有所改进。