/* GUI definition
 *
 * part of this code comes from SimpleWatch example
 */

#include "config.h"
#include "gui.h"

EventGroupHandle_t g_event_group;
QueueHandle_t g_event_queue_handle;

LV_FONT_DECLARE(Geometr);
LV_FONT_DECLARE(Ubuntu);
LV_IMG_DECLARE(bg);
// LV_IMG_DECLARE(bg1);
// LV_IMG_DECLARE(bg2);
// LV_IMG_DECLARE(bg3);
LV_IMG_DECLARE(WALLPAPER_1_IMG);
LV_IMG_DECLARE(WALLPAPER_2_IMG);
LV_IMG_DECLARE(WALLPAPER_3_IMG);
LV_IMG_DECLARE(step);
LV_IMG_DECLARE(menu);

LV_IMG_DECLARE(wifi);
LV_IMG_DECLARE(light);
LV_IMG_DECLARE(bluetooth);
// LV_IMG_DECLARE(sd);
LV_IMG_DECLARE(setting);
LV_IMG_DECLARE(on);
LV_IMG_DECLARE(off);
LV_IMG_DECLARE(level1);
LV_IMG_DECLARE(level2);
LV_IMG_DECLARE(level3);
LV_IMG_DECLARE(iexit);
LV_IMG_DECLARE(modules);
// LV_IMG_DECLARE(CAMERA_PNG);

static lv_style_t settingStyle;
static lv_obj_t *mainBar = nullptr;
static lv_obj_t *timeLabel = nullptr;
static lv_obj_t *menuBtn = nullptr;

static uint8_t globalIndex = 0;

class StatusBar {
    typedef struct {
        bool vaild;
        lv_obj_t *icon;
    } lv_status_bar_t;

    lv_obj_t *_bar = nullptr;
    lv_obj_t *_par = nullptr;
    uint8_t _barHeight = 30;
    lv_status_bar_t _array[6];
    const int8_t iconOffset = -5;

public:
    StatusBar(){
        memset(_array, 0, sizeof(_array));
    }

    void createIcons(lv_obj_t *par){
        _par = par;

        static lv_style_t barStyle;

        lv_style_init(&barStyle);
        lv_style_set_radius(&barStyle, LV_OBJ_PART_MAIN, 0);
        lv_style_set_bg_color(&barStyle, LV_OBJ_PART_MAIN, LV_COLOR_GRAY);
        lv_style_set_bg_opa(&barStyle, LV_OBJ_PART_MAIN, LV_OPA_20);
        lv_style_set_border_width(&barStyle, LV_OBJ_PART_MAIN, 0);
        lv_style_set_text_color(&barStyle, LV_OBJ_PART_MAIN, LV_COLOR_WHITE);
        lv_style_set_image_recolor(&barStyle, LV_OBJ_PART_MAIN, LV_COLOR_WHITE);

        _bar = lv_cont_create(_par, NULL);
        lv_obj_set_size(_bar,  LV_HOR_RES, _barHeight);
        lv_obj_add_style(_bar, LV_OBJ_PART_MAIN, &barStyle);

        _array[0].icon = lv_label_create(_bar, NULL);
        lv_label_set_text(_array[0].icon, "100%");

        _array[1].icon = lv_img_create(_bar, NULL);
        lv_img_set_src(_array[1].icon, LV_SYMBOL_BATTERY_FULL);

        _array[2].icon = lv_img_create(_bar, NULL);
        lv_img_set_src(_array[2].icon, LV_SYMBOL_WIFI);
        lv_obj_set_hidden(_array[2].icon, true);

        _array[3].icon = lv_img_create(_bar, NULL);
        lv_img_set_src(_array[3].icon, LV_SYMBOL_BLUETOOTH);
        lv_obj_set_hidden(_array[3].icon, true);

        //step counter
        _array[4].icon = lv_img_create(_bar, NULL);
        lv_img_set_src(_array[4].icon, &step);
        lv_obj_align(_array[4].icon, _bar, LV_ALIGN_IN_LEFT_MID, 10, 0);

        _array[5].icon = lv_label_create(_bar, NULL);
        lv_label_set_text(_array[5].icon, "0");
        lv_obj_align(_array[5].icon, _array[4].icon, LV_ALIGN_OUT_RIGHT_MID, 5, 0);

        refresh();
    }

    void setStepCounter(uint32_t counter){
        lv_label_set_text(_array[5].icon, String(counter).c_str());
        lv_obj_align(_array[5].icon, _array[4].icon, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
    }

    void updateLevel(int level){
        lv_label_set_text(_array[0].icon, (String(level) + "%").c_str());
        refresh();
    }

    void updateBatteryIcon(lv_icon_battery_t icon){
        const char *icons[6] = {LV_SYMBOL_BATTERY_EMPTY, LV_SYMBOL_BATTERY_1, LV_SYMBOL_BATTERY_2, LV_SYMBOL_BATTERY_3, LV_SYMBOL_BATTERY_FULL, LV_SYMBOL_CHARGE};
        lv_img_set_src(_array[1].icon, icons[icon]);
        refresh();
    }

    void show(lv_icon_status_bar_t icon){
        lv_obj_set_hidden(_array[icon].icon, false);
        refresh();
    }

    void hidden(lv_icon_status_bar_t icon){
        lv_obj_set_hidden(_array[icon].icon, true);
        refresh();
    }

    uint8_t height(){
        return _barHeight;
    }

    lv_obj_t *self(){
        return _bar;
    }

private:
    void refresh(){
        int prev;
        for(int i = 0; i < 4; i++){
            if(!lv_obj_get_hidden(_array[i].icon)){
                if(i == LV_STATUS_BAR_BATTERY_LEVEL)
                    lv_obj_align(_array[i].icon, NULL, LV_ALIGN_IN_RIGHT_MID, 0, 0);
                else
                    lv_obj_align(_array[i].icon, _array[prev].icon, LV_ALIGN_OUT_LEFT_MID, iconOffset, 0);
                prev = i;
            }
        }
    }
};

class MenuBar {
    lv_obj_t *_cont, *_view, *_exit, * *_obj;
    lv_point_t *_vp ;
    int _count = 0;

public:
    typedef struct {
        const char *name;
        void *img;
        void (*event_cb)();
    } lv_menu_config_t;

    MenuBar(){
        _cont = nullptr;
        _view = nullptr;
        _exit = nullptr;
        _obj = nullptr;
        _vp = nullptr;
    };
    ~MenuBar(){};

    void createMenu(lv_menu_config_t *config, int count, lv_event_cb_t event_cb, int direction = 1){
        static lv_style_t menuStyle;
        lv_style_init(&menuStyle);
        lv_style_set_radius(&menuStyle, LV_OBJ_PART_MAIN, 0);
        lv_style_set_bg_color(&menuStyle, LV_OBJ_PART_MAIN, LV_COLOR_GRAY);
        lv_style_set_bg_opa(&menuStyle, LV_OBJ_PART_MAIN, LV_OPA_0);
        lv_style_set_border_width(&menuStyle, LV_OBJ_PART_MAIN, 0);
        lv_style_set_text_color(&menuStyle, LV_OBJ_PART_MAIN, LV_COLOR_WHITE);
        lv_style_set_image_recolor(&menuStyle, LV_OBJ_PART_MAIN, LV_COLOR_WHITE);


        _count = count;

        _vp = new lv_point_t [count];

        _obj = new lv_obj_t *[count];

        for(int i = 0; i < count; i++){
            if(direction){
                _vp[i].x = 0;
                _vp[i].y = i;
            } else {
                _vp[i].x = i;
                _vp[i].y = 0;
            }
        }

        _cont = lv_cont_create(lv_scr_act(), NULL);
        lv_obj_set_size(_cont, LV_HOR_RES, LV_VER_RES - 30);
        lv_obj_align(_cont, NULL, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
        lv_obj_add_style(_cont, LV_OBJ_PART_MAIN, &menuStyle);

        _view = lv_tileview_create(_cont, NULL);
        lv_tileview_set_valid_positions(_view, _vp, count );
        lv_tileview_set_edge_flash(_view, false);
        lv_obj_align(_view, NULL, LV_ALIGN_CENTER, 0, 0);
        lv_page_set_scrlbar_mode(_view, LV_SCRLBAR_MODE_OFF);
        lv_obj_add_style(_view, LV_OBJ_PART_MAIN, &menuStyle);

        lv_coord_t _w = lv_obj_get_width(_view) ;
        lv_coord_t _h = lv_obj_get_height(_view);

        for(int i = 0; i < count; i++){
            _obj[i] = lv_cont_create(_view, _view);
            lv_obj_set_size(_obj[i], _w, _h);

            lv_obj_t *img = lv_img_create(_obj[i], NULL);
            lv_img_set_src(img, config[i].img);
            lv_obj_align(img, _obj[i], LV_ALIGN_CENTER, 0, 0);

            lv_obj_t *label = lv_label_create(_obj[i], NULL);
            lv_label_set_text(label, config[i].name);
            lv_obj_align(label, img, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);


            i == 0 ? lv_obj_align(_obj[i], NULL, LV_ALIGN_CENTER, 0, 0) : lv_obj_align(_obj[i], _obj[i - 1], direction ? LV_ALIGN_OUT_BOTTOM_MID : LV_ALIGN_OUT_RIGHT_MID, 0, 0);

            lv_tileview_add_element(_view, _obj[i]);
            lv_obj_set_click(_obj[i], true);
            lv_obj_set_event_cb(_obj[i], event_cb);
        }

        _exit  = lv_imgbtn_create(lv_scr_act(), NULL);
        lv_imgbtn_set_src(_exit, LV_BTN_STATE_RELEASED, &menu);
        lv_imgbtn_set_src(_exit, LV_BTN_STATE_PRESSED, &menu);
        lv_imgbtn_set_src(_exit, LV_BTN_STATE_CHECKED_PRESSED, &menu);
        lv_imgbtn_set_src(_exit, LV_BTN_STATE_CHECKED_RELEASED, &menu);
        lv_obj_align(_exit, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, -20, -20);
        lv_obj_set_event_cb(_exit, event_cb);
        lv_obj_set_top(_exit, true);
    }

    lv_obj_t *exitBtn() const {
        return _exit;
    }

    lv_obj_t *self() const {
        return _cont;
    }

    void hidden(bool en = true){
        lv_obj_set_hidden(_cont, en);
        lv_obj_set_hidden(_exit, en);
    }

    lv_obj_t *obj(int index) const {
        if (index > _count)return nullptr;
        return _obj[index];
    }
};

MenuBar::lv_menu_config_t _cfg[] = {
//    {.name = "WiFi",  .img = (void *) &wifi, .event_cb = wifi_event_cb},
    {.name = "Bluetooth",  .img = (void *) &bluetooth, /*.event_cb = bluetooth_event_cb*/},
    {.name = "Light",  .img = (void *) &light, /*.event_cb = light_event_cb*/},
    {.name = "Setting",  .img = (void *) &setting, /*.event_cb = setting_event_cb */},
//    {.name = "Modules",  .img = (void *) &modules, /*.event_cb = modules_event_cb */},
};

MenuBar menuBars;
StatusBar bar;


