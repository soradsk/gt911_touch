# gt911_touch
基于gt911电容触摸芯片的touch函数
主要运行函数为void touch_run();
通过Coordinate_Touch结构体获取touch的状态
Coordinate_Touch：x轴、y轴坐标，当前速度v，start_x、start_y触摸开始坐标，type触摸类型（TOUCH_TYPE_CLICK，TOUCH_TYPE_LONG_CLICK），signal当前状态（TOUCH_START，TOUCH_MOVE，TOUCH_END），
count用于区分LONG_CLICK和CLICK
