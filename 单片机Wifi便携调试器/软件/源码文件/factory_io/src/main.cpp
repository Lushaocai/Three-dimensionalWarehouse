#include <lvgl.h>
#include <TFT_eSPI.h>
#include <../src/LVGL/ui.h>
#include <WiFi.h>
#include <ModbusIP_ESP8266.h>
// modbus IP
ModbusIP mb;

/*Change to your screen resolution*/
static const uint16_t screenWidth = 320;
static const uint16_t screenHeight = 240;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[screenWidth * 10];

TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight); /* TFT instance */

#if LV_USE_LOG != 0
/* Serial debugging */
void my_print(const char *buf)
{
    Serial.printf(buf);
    Serial.flush();
}
#endif

/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors((uint16_t *)&color_p->full, w * h, true);
    tft.endWrite();

    lv_disp_flush_ready(disp);
}

/*Read the touchpad*/
void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
    uint16_t touchX, touchY;

    bool touched = tft.getTouch(&touchX, &touchY, 600);

    if (!touched)
    {
        data->state = LV_INDEV_STATE_REL;
    }
    else
    {
        data->state = LV_INDEV_STATE_PR;

        /*Set the coordinates*/
        data->point.x = map(touchX, 0, 240, 0, 320);
        data->point.y = map(touchY, 0, 320, 0, 240);

        // Serial.print( "Data x " );
        // Serial.println( map(touchX,0,240,0,320) );

        // Serial.print( "Data y " );
        // Serial.println( map(touchY,0,320,0,240) );
    }
}

void setup()
{
    Serial.begin(115200); /* prepare for possible serial debug */
    IPAddress staticIP(192,168,97,97);
    IPAddress gateway(192,168,97,1);
    IPAddress subnet(255,255,255,0);
    
    // Wifi And ModbusIP Begin
    WiFi.config(staticIP,gateway,subnet);
    WiFi.begin("MEIZU 18s", "qwertyuiop");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    mb.server(502);
    mb.addCoil(0, 0, 45);

    String LVGL_Arduino = "Hello Arduino! ";
    LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();

    Serial.println(LVGL_Arduino);
    Serial.println("I am LVGL_Arduino");

    lv_init();

#if LV_USE_LOG != 0
    lv_log_register_print_cb(my_print); /* register print function for debugging */
#endif

    tft.begin();        /* TFT init */
    tft.setRotation(1); /* Landscape orientation, flipped */
    /*Set the touchscreen calibration data,
     the actual data for your display can be acquired using
     the Generic -> Touch_calibrate example from the TFT_eSPI library*/

    uint16_t calData[5] = {280, 3528, 317, 3368, 1};
    tft.setTouch(calData);

    lv_disp_draw_buf_init(&draw_buf, buf, NULL, screenWidth * 10);

    /*Initialize the display*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    /*Change the following line to your display resolution*/
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    /*Initialize the (dummy) input device driver*/
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register(&indev_drv);

    ui_init();

    Serial.println("Setup done");
}

// 1号仓
extern int List1;              // 执行器选择
extern bool TEST_Btn1;         // 测试按钮
extern bool TEST_Enable1;      // 测试使能
extern bool Material1_sensor1; // 入料传感器
extern bool Material1_sensor2; // 出料传感器
extern bool BOX1_sensor1;      // 货箱到达传感器
extern bool BOX1_sensor2;      // 货箱进入传感器
extern bool BOX1_sensor3;      // 货箱离开传感器
// 2号仓
extern int List2;              // 执行器选择
extern bool TEST_Btn2;         // 测试按钮
extern bool TEST_Enable2;      // 测试使能
extern bool Material2_sensor1; // 入料传感器
extern bool Material2_sensor2; // 出料传感器
extern bool BOX2_sensor1;      // 货箱到达传感器
extern bool BOX2_sensor2;      // 货箱进入传感器
extern bool BOX2_sensor3;      // 货箱离开传感器
// 3号仓
extern int List3;              // 执行器选择
extern bool TEST_Btn3;         // 测试按钮
extern bool TEST_Enable3;      // 测试使能
extern bool Material3_sensor1; // 入料传感器
extern bool Material3_sensor2; // 出料传感器
extern bool BOX3_sensor1;      // 货箱到达传感器
extern bool BOX3_sensor2;      // 货箱进入传感器
extern bool BOX3_sensor3;      // 货箱离开传感器

void loop()
{
    mb.task();
    lv_timer_handler(); /* let the GUI do its work */
    delay(5);
    MyTask();
    // 1号仓
    Material1_sensor1 = mb.Coil(8);  // 入料传感器
    Material1_sensor2 = mb.Coil(9); // 出料传感器
    BOX1_sensor1 = mb.Coil(10);      // 货箱到达传感器
    BOX1_sensor2 = mb.Coil(11);      // 货箱进入传感器
    BOX1_sensor3 = mb.Coil(12);      // 货箱离开传感器
                                     // 2号仓
    Material2_sensor1 = mb.Coil(21); // 入料传感器
    Material2_sensor2 = mb.Coil(22); // 出料传感器
    BOX2_sensor1 = mb.Coil(23);      // 货箱到达传感器
    BOX2_sensor2 = mb.Coil(24);      // 货箱进入传感器
    BOX2_sensor3 = mb.Coil(25);      // 货箱离开传感器
                                     // 3号仓
    Material3_sensor1 = mb.Coil(34); // 入料传感器
    Material3_sensor2 = mb.Coil(35); // 出料传感器
    BOX3_sensor1 = mb.Coil(36);      // 货箱到达传感器
    BOX3_sensor2 = mb.Coil(37);      // 货箱进入传感器
    BOX3_sensor3 = mb.Coil(38);      // 货箱离开传感器



    if (TEST_Enable1)
    {
        if (TEST_Btn1)
        {
            switch (List1)
            {
            case 0:
                mb.Coil(0, 1);
                break;
            case 1:
                mb.Coil(1, 1);
                break;
            case 2:
                mb.Coil(2, 1);
                break;
            case 3:
                mb.Coil(3, 1);
                break;
            case 4:
                mb.Coil(4, 1);
                break;
            case 5:
                mb.Coil(5, 1);
                break;
            case 6:
                mb.Coil(6, 1);
                break;
            case 7:
                mb.Coil(7, 1);
                break;
            }
        }
        else
        {
            mb.Coil(0, 0);            
            mb.Coil(1, 0);
            mb.Coil(2, 0);
            mb.Coil(3, 0);
            mb.Coil(4, 0);
            mb.Coil(5, 0);
            mb.Coil(6, 0);
            mb.Coil(7, 0);

        }
    }

    if (TEST_Enable2)
    {
        if (TEST_Btn2)
        {
            switch (List2)
            {
            case 0:
                mb.Coil(13, 1);
                break;
            case 1:
                mb.Coil(14, 1);
                break;
            case 2:
                mb.Coil(15, 1);
                break;
            case 3:
                mb.Coil(16, 1);
                break;
            case 4:
                mb.Coil(17, 1);
                break;
            case 5:
                mb.Coil(18, 1);
                break;
            case 6:
                mb.Coil(29, 1);
                break;
            case 7:
                mb.Coil(20, 1);
                break;
            }
        }
        else
        {
            mb.Coil(13, 0);            
            mb.Coil(14, 0);
            mb.Coil(15, 0);
            mb.Coil(16, 0);
            mb.Coil(17, 0);
            mb.Coil(18, 0);
            mb.Coil(19, 0);
            mb.Coil(20, 0);

        }
    }

    if (TEST_Enable3)
    {
        if (TEST_Btn3)
        {
            switch (List3)
            {
            case 0:
                mb.Coil(26, 1);
                break;
            case 1:
                mb.Coil(27, 1);
                break;
            case 2:
                mb.Coil(28, 1);
                break;
            case 3:
                mb.Coil(29, 1);
                break;
            case 4:
                mb.Coil(30, 1);
                break;
            case 5:
                mb.Coil(31, 1);
                break;
            case 6:
                mb.Coil(3, 1);
                break;
            case 7:
                mb.Coil(33, 1);
                break;
            }
        }
        else
        {
            mb.Coil(26, 0);            
            mb.Coil(27, 0);
            mb.Coil(28, 0);
            mb.Coil(29, 0);
            mb.Coil(30, 0);
            mb.Coil(31, 0);
            mb.Coil(32, 0);
            mb.Coil(33, 0);

        }
    }
}