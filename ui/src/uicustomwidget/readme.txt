1、将libs目录下相应的控件库目录下的插件dll和lib拷贝到D:\Qt\4.8.6\plugins\designer下
   打开qtdesigner，可在自定义窗口部件中看到相应控件(在qtcreator的"设计"页看不到)
2、CustomWidget为普通动态库，使用时在pro中添加库依赖
   LIBS += -L$${DESTDIR} 
           -laxisstatus \
           -lcylinder \
           -luicustomwidget
   new CustomWidget对象，并与axisstatus对象或cylinder对象的相应信号连接。
   在UI上拖拽自定义控件时会自动生成对象，也可以自己new控件，生成对象。
3、自定义控件为静态库，lib文件放在debug32下，头文件在uicustomwidget内。