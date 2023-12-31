/*
 * Copyright (C) 2012-2015 HUAWEI, Inc.
 * Author: HUAWEI, Inc.
 *
 * This package is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/module.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/of.h>
#include <linux/io.h>
#include <linux/jiffies.h>
#include <linux/gpio.h>
#include <linux/of_gpio.h>
#include <linux/platform_device.h>
#include <huawei_platform/log/hw_log.h>
#include <linux/notifier.h>
#include <linux/wakelock.h>
#include <linux/hisi/usb/hisi_usb.h>
#include <linux/timer.h>
#include <linux/hrtimer.h>
#include <dsm/dsm_pub.h>
#include <linux/hisi/hisi_adc.h>
#include <linux/delay.h>
#ifdef CONFIG_DIRECT_CHARGER
#include <huawei_platform/power/direct_charger.h>
#endif
#include <linux/power/hisi/coul/hisi_coul_drv.h>
#include <huawei_platform/power/huawei_charger.h>
#include <huawei_platform/power/usb_short_circuit_protect.h>

HWLOG_REGIST();

static bool uscp_enable = true;
static bool uscp_probe_ok = false;
module_param(uscp_enable, bool, USCP_ENABLE_PAR);//uscp enable parameter 0644

static int usb_temp = USB_TEMP_NUM;//USB temperature 25 centigrade
module_param(usb_temp, int, USB_TEMP_PAR);//USB temperature parameter 0444

static bool uscp_enforce = false;
module_param(uscp_enforce, bool, USCP_ENFORCE_PAR);//uscp enforce parameter 0644

struct uscp_device_info
{
    struct device   *dev;
    struct workqueue_struct *uscp_wq;
    struct work_struct uscp_check_wk;
    struct notifier_block   usb_nb;
    struct notifier_block tcpc_nb;
    struct hrtimer timer;
    int gpio_uscp;
    int adc_channel_uscp;
    int uscp_threshold_tusb;
    int open_mosfet_temp;
    int open_hiz_temp;
    int close_mosfet_temp;
    int interval_switch_temp;
    int check_interval;
    int keep_check_cnt;
    int dmd_hiz_enable;
};

static struct dsm_dev dsm_uscp =
{
    .name = "dsm_usb_short_circuit_protect",
    .fops = NULL,
    .buff_size = DSM_USCP_BUFFSIZE,//dsm uscp buffize 1024
};
static int protect_enable = 0;
static int protect_dmd_notify_enable = 1;
static int protect_dmd_notify_enable_hiz = DMD_NOTIFY_HIZ_ENABLE;
static int is_uscp_mode = 0;
static int is_hiz_mode = NOT_HIZ_MODE;
static int is_rt_uscp_mode = 0;
static unsigned int first_in = 1;
#ifdef CONFIG_DIRECT_CHARGER
static int is_scp_charger = 0;
#endif
static int T_A_TABLE[][2] =
{
  {-40, 3851},
  {-39, 3838},
  {-38, 3825},
  {-37, 3811},
  {-36, 3796},
  {-35, 3782},
  {-34, 3766},
  {-33, 3750},
  {-32, 3733},
  {-31, 3716},
  {-30, 3697},
  {-29, 3679},
  {-28, 3659},
  {-27, 3639},
  {-26, 3619},
  {-25, 3597},
  {-24, 3575},
  {-23, 3552},
  {-22, 3529},
  {-21, 3505},
  {-20, 3480},
  {-19, 3454},
  {-18, 3428},
  {-17, 3401},
  {-16, 3373},
  {-15, 3345},
  {-14, 3316},
  {-13, 3286},
  {-12, 3256},
  {-11, 3225},
  {-10, 3193},
  {-9, 3161},
  {-8, 3128},
  {-7, 3095},
  {-6, 3060},
  {-5, 3026},
  {-4, 2990},
  {-3, 2955},
  {-2, 2919},
  {-1, 2882},
  {0, 2845},
  {1, 2807},
  {2, 2769},
  {3, 2731},
  {4, 2692},
  {5, 2653},
  {6, 2614},
  {7, 2575},
  {8, 2535},
  {9, 2495},
  {10, 2455},
  {11, 2416},
  {12, 2376},
  {13, 2335},
  {14, 2295},
  {15, 2255},
  {16, 2215},
  {17, 2175},
  {18, 2135},
  {19, 2096},
  {20, 2056},
  {21, 2017},
  {22, 1977},
  {23, 1938},
  {24, 1900},
  {25, 1862},
  {26, 1824},
  {27, 1786},
  {28, 1749},
  {29, 1713},
  {30, 1676},
  {31, 1640},
  {32, 1605},
  {33, 1570},
  {34, 1535},
  {35, 1501},
  {36, 1468},
  {37, 1435},
  {38, 1402},
  {39, 1370},
  {40, 1339},
  {41, 1308},
  {42, 1277},
  {43, 1247},
  {44, 1218},
  {45, 1189},
  {46, 1161},
  {47, 1134},
  {48, 1107},
  {49, 1080},
  {50, 1054},
  {51, 1029},
  {52, 1004},
  {53, 979},
  {54, 955},
  {55, 932},
  {56, 909},
  {57, 887},
  {58, 866},
  {59, 844},
  {60, 823},
  {61, 803},
  {62, 783},
  {63, 764},
  {64, 745},
  {65, 727},
  {66, 709},
  {67, 691},
  {68, 674},
  {69, 658},
  {70, 641},
  {71, 626},
  {72, 610},
  {73, 595},
  {74, 580},
  {75, 566},
  {76, 552},
  {77, 539},
  {78, 525},
  {79, 512},
  {80, 500},
  {81, 488},
  {82, 476},
  {83, 464},
  {84, 453},
  {85, 442},
  {86, 431},
  {87, 421},
  {88, 411},
  {89, 401},
  {90, 391},
  {91, 382},
  {92, 373},
  {93, 364},
  {94, 355},
  {95, 347},
  {96, 338},
  {97, 331},
  {98, 323},
  {99, 315},
  {100, 308},
  {101, 301},
  {102, 294},
  {103, 287},
  {104, 280},
  {105, 274},
  {106, 268},
  {107, 261},
  {108, 255},
  {109, 250},
  {110, 244},
  {111, 238},
  {112, 233},
  {113, 228},
  {114, 223},
  {115, 218},
  {116, 213},
  {117, 208},
  {118, 204},
  {119, 199},
  {120, 195},
  {121, 191},
  {122, 186},
  {123, 182},
  {124, 178},
  {125, 175},
};

static struct dsm_client *uscp_client = NULL;
static struct uscp_device_info* g_di = NULL;
static struct wake_lock uscp_wakelock;

static void uscp_wake_lock(void)
{
    if(!wake_lock_active(&uscp_wakelock))
    {
        wake_lock(&uscp_wakelock);
    }
}

static void uscp_wake_unlock(void)
{
    if(wake_lock_active(&uscp_wakelock))
    {
        wake_unlock(&uscp_wakelock);
    }
}

static void charge_type_handler(struct uscp_device_info* di, enum hisi_charger_type type)
{
    int interval = 0;

    if (!protect_enable)
        return;
    if ((CHARGER_TYPE_DCP == type) || (CHARGER_TYPE_UNKNOWN == type)
        ||(CHARGER_TYPE_SDP == type) ||(CHARGER_TYPE_CDP == type))
    {
        if (hrtimer_active(&(di->timer)))
        {
        }
        else
        {
            interval = 0;
            first_in = 1;
            /*record 30 seconds after the charger just insert; 30s = (1100 - 1001 + 1)*300ms */
            di->keep_check_cnt = CHECK_CNT_INIT;//keep check count init number 1100
            hrtimer_start(&di->timer, ktime_set(interval/MSEC_PER_SEC, (interval % MSEC_PER_SEC) * USEC_PER_SEC), HRTIMER_MODE_REL);
        }
    }
    else
    {
    }
}

static int uscp_notifier_call(struct notifier_block *usb_nb, unsigned long event, void *data)
{
    struct uscp_device_info *di = container_of(usb_nb, struct uscp_device_info, usb_nb);
    enum hisi_charger_type type = ((enum hisi_charger_type)event);

    charge_type_handler(di, type);
    return NOTIFY_OK;
}
#ifdef CONFIG_TCPC_CLASS
static int pd_notifier_call(struct notifier_block *usb_nb, unsigned long event, void *data)
{
    return uscp_notifier_call(usb_nb, event, data);
}
#endif
static int usb_notifier_call(struct notifier_block *usb_nb, unsigned long event, void *data)
{
    return uscp_notifier_call(usb_nb, event, data);
}

static int get_one_adc_sample(void)
{
    int i =0;
    const int retry_times = 3;
    int T_sample = -1;
    struct uscp_device_info* di = g_di;

    for (i = 0; i < retry_times; ++i)
    {
        T_sample = hisi_adc_get_adc(di->adc_channel_uscp);
        if (T_sample < 0)
        {
            hwlog_err("adc read fail!\n");
        }
        else
        {
            break;
        }
    }
    return T_sample;
}
static int adc_to_temp(int adc_value)
{
    int table_size = sizeof(T_A_TABLE)/sizeof(T_A_TABLE[0]);
    int high = table_size - 1;
    int low = 0;
    int mid = 0;

    if (adc_value >= T_A_TABLE[0][1])
        return T_A_TABLE[0][0];
    if (adc_value <= T_A_TABLE[table_size - 1][1])
        return T_A_TABLE[table_size - 1][0];
    /*use binary search*/
    while (low < high)
    {
        hwlog_debug("low = %d,high = %d!\n", low, high);
        mid = (low + high) / 2;
        if (0 == mid)
            return T_A_TABLE[1][0];
        if (adc_value > T_A_TABLE[mid][1])
        {
            if (adc_value < T_A_TABLE[mid - 1][1])
                return T_A_TABLE[mid][0];
            high = mid - 1;
        }
        else if(adc_value < T_A_TABLE[mid][1])
        {
            if (adc_value >= T_A_TABLE[mid + 1][1])
                return T_A_TABLE[mid + 1][0];
            low = mid + 1;
        }
        else
            return T_A_TABLE[mid][0];
    }
    hwlog_err("transform error!\n");
    return 0;
}

#define USB_SHORT_NTC_SAMPLES  (3)
#define USB_SHORT_NTC_INVALID_TEMP_THRE  (10)

static int get_temperature_value(void)
{
    int i = 0;
    int j = 0;

    int adc_temp = 0;
    int temp_array[USB_SHORT_NTC_SAMPLES] = {0};

    int temp_samples = 3;
    int temp_invalid_flag = 0;

    int sum = 0;

    while (temp_samples--)
    {
        temp_invalid_flag = 0;

        for (i = 0; i < USB_SHORT_NTC_SAMPLES; ++i)
        {
            adc_temp = get_one_adc_sample();
            temp_array[i] = adc_to_temp(adc_temp);

            if (adc_temp < 0)
            {
                temp_invalid_flag = 1;
            }
        }

        if (temp_invalid_flag == 1) {
            continue;
        }

        /* check temperature value is valid */
        for (i = 0; i < (USB_SHORT_NTC_SAMPLES - 1); ++i)
        {
            for (j = (i + 1); j < USB_SHORT_NTC_SAMPLES; ++j)
            {
                if (abs(temp_array[i] - temp_array[j]) > USB_SHORT_NTC_INVALID_TEMP_THRE)
                {
                    hwlog_err("invalid temperature temp[%d]=%d temp[%d]=%d!\n", i, temp_array[i], j, temp_array[j]);
                    temp_invalid_flag = 1;
                    break;
                }
            }
        }

        if (temp_invalid_flag == 0) {
            break;
        }
    }

    if (temp_invalid_flag == 0)
    {
        /* get average temperature */
        for (i = 0; i < USB_SHORT_NTC_SAMPLES; ++i)
        {
            sum += temp_array[i];
        }
        return (sum/USB_SHORT_NTC_SAMPLES);
    }
    else
    {
        hwlog_err("use 0 as default temperature!\n");
        return 0;
    }
}
static void set_interval(struct uscp_device_info* di, int temp)
{
    if(NULL == di)
    {
        hwlog_err("di is NULL\n");
        return;
    }
    if (temp > di->interval_switch_temp) {
        di->check_interval = CHECK_INTERVAL_300;//set the check interval 300
        di->keep_check_cnt = 0;
    } else {
        if (di->keep_check_cnt > CHECK_CNT_LIMIT) {
            /*check the temperature per 0.3 second for 100 times ,when the charger just insert.*/
            di->keep_check_cnt -= 1;
            di->check_interval = CHECK_INTERVAL_300;
            is_uscp_mode = 0;
        } else if (di->keep_check_cnt == CHECK_CNT_LIMIT) {
            /* reset the flag when the temperature status is stable*/
            di->keep_check_cnt = -1;
            di->check_interval = CHECK_INTERVAL_10000;//set the check interval 10000
            is_uscp_mode = 0;
            uscp_wake_unlock();
        } else if (di->keep_check_cnt >= 0) {
            di->keep_check_cnt = di->keep_check_cnt + 1;
            di->check_interval = CHECK_INTERVAL_300;
        } else {
            di->check_interval = CHECK_INTERVAL_10000;
            is_uscp_mode = 0;
        }
    }
}
static void protection_process(struct uscp_device_info* di, int tbatt, int tusb)
{
    int ret = 0;
    int state = 0;
    int tdiff = 0;

    if(NULL == di)
    {
        hwlog_err("di is NULL\n");
        return;
    }
    if (!uscp_enable) {
        return;
    }
    tdiff = tusb - tbatt;

    if ((tusb >= di->uscp_threshold_tusb) && (tdiff >= di->open_hiz_temp)){
        is_hiz_mode = HIZ_MODE;
        hwlog_err("enable charge hiz!\n");
        charge_set_hiz_enable(HIZ_ENABLE);
    }

    if (((tusb >= di->uscp_threshold_tusb) && (tdiff >= di->open_mosfet_temp)) || (uscp_enforce)) {
        uscp_wake_lock();
        is_uscp_mode = 1;
        is_rt_uscp_mode = 1;
        if (uscp_enforce)
            hwlog_err("uscp_enforce=%d, force usb circuit protect work!\n", uscp_enforce);
#ifdef CONFIG_DIRECT_CHARGER
        scp_set_stop_charging_flag(1);
        state = scp_get_stage_status();
        while(1) {
            state = scp_get_stage_status();
            if (is_scp_stop_charging_complete() &&
                ((SCP_STAGE_DEFAULT == state) ||(SCP_STAGE_CHARGE_DONE == state)))
                    break;
            }
        scp_set_stop_charging_flag(0);
        if (SCP_STAGE_DEFAULT == state) {
            if (first_in) {
                if (SCP_ADAPTOR_DETECT_SUCC == scp_adaptor_detect()) {
                    is_scp_charger = 1;
                } else {
                    is_scp_charger = 0;
                }
                first_in = 0;
            }
        } else if(SCP_STAGE_CHARGE_DONE == state) {
            is_scp_charger = 1;
        }else {
            /*do nothing*/
        }

        if(is_scp_charger) {
            ret = scp_adaptor_set_output_enable(0);
            if (!ret) {
                hwlog_err("disable scp adaptor output success!\n");
                msleep(SLEEP_200MS);
            } else {
                hwlog_err("disable scp adaptor output fail!\n");
            }
        }
#endif
        msleep(SLEEP_10MS);
        gpio_set_value(di->gpio_uscp, 1);/*open mosfet*/
        hwlog_err("pull up gpio_uscp!\n");
    } else if (tdiff <= di->close_mosfet_temp) {
#ifdef CONFIG_DIRECT_CHARGER
        if (is_scp_charger) {
            ret = scp_adaptor_set_output_enable(1);
            if (!ret) {
                hwlog_err("enable scp adaptor output success!\n");
            } else {
                    hwlog_err("enable scp adaptor output fail!\n");
            }
        }
#endif
        if (is_uscp_mode) {
            gpio_set_value(di->gpio_uscp, 0);/*close mosfet*/
            is_rt_uscp_mode = 0;
            msleep(SLEEP_10MS);
            charge_set_hiz_enable(HIZ_DISABLE);
            is_hiz_mode = NOT_HIZ_MODE;
        }
        if(is_hiz_mode){
            charge_set_hiz_enable(HIZ_DISABLE);
            is_hiz_mode = NOT_HIZ_MODE;
        }
    } else {
        /*do nothing*/
    }
}
static void check_temperature(struct uscp_device_info* di)
{
    int tusb = 0;
    int tbatt = 0;
    int tdiff = 0;
    int batt_id = 0;

    if(NULL == di)
    {
        hwlog_err("di is NULL\n");
        return;
    }
    tusb = get_temperature_value();
    usb_temp = tusb;
    tbatt = hisi_battery_temperature();
    tdiff = tusb - tbatt;

    if (di->dmd_hiz_enable) {
        if ((tusb >= di->uscp_threshold_tusb) && (tdiff >= di->open_hiz_temp)) {
#ifdef CONFIG_HUAWEI_DSM
            if (protect_dmd_notify_enable_hiz) {
                if (!dsm_client_ocuppy(uscp_client)) {
                    dsm_client_record(uscp_client, "usb short happened,open hiz!\n");
                    dsm_client_notify(uscp_client, ERROR_NO_USB_SHORT_PROTECT_HIZ);
                    protect_dmd_notify_enable_hiz = DMD_NOTIFY_HIZ_DISABLE;
                }
            }
#endif
        }
    }

    if ((tusb >= di->uscp_threshold_tusb) && (tdiff >= di->open_mosfet_temp)) {
        is_rt_uscp_mode = 1;
#ifdef CONFIG_HUAWEI_DSM
        if (protect_dmd_notify_enable) {
            if (!dsm_client_ocuppy(uscp_client)) {
                batt_id = hisi_battery_id_voltage();
                dsm_client_record(uscp_client, "usb short happened,tusb = %d,tbatt = %d,batt_id = %d\n",
                    tusb,tbatt,batt_id);
                dsm_client_notify(uscp_client, ERROR_NO_USB_SHORT_PROTECT);
                protect_dmd_notify_enable = 0;
            }
        }
#endif
    }

    set_interval(di, tdiff);
    protection_process(di, tbatt, tusb);
}
int is_in_uscp_mode(void)
{
    return is_uscp_mode;
}
int is_uscp_hiz_mode(void)
{
    return is_hiz_mode;
}
int is_in_rt_uscp_mode(void)
{
    return is_rt_uscp_mode;
}
int get_usb_ntc_temp(void)
{
    if(true == uscp_probe_ok){
        return get_temperature_value();
    }

    return INVALID_TEMP_VAL;//invalid temp flag
}
static void uscp_check_work(struct work_struct *work)
{
    struct uscp_device_info *di = container_of(work,struct uscp_device_info, uscp_check_wk);
    int interval = 0;
    enum hisi_charger_type type = hisi_get_charger_type();
#ifdef CONFIG_DIRECT_CHARGER
    if ((-1 == di->keep_check_cnt) && (CHARGER_TYPE_NONE == type) &&
        SCP_STAGE_DEFAULT == scp_get_stage_status())
#else
    if ((-1 == di->keep_check_cnt) && (CHARGER_TYPE_NONE == type))
#endif
    {
        protect_dmd_notify_enable = 1;
        gpio_set_value(di->gpio_uscp, 0);/*close mosfet*/
        di->keep_check_cnt = -1;
        di->check_interval = CHECK_INTERVAL_10000;
        is_uscp_mode = 0;
        di->keep_check_cnt = CHECK_CNT_INIT;/*check count init number 1100*/
        first_in = 1;
#ifdef CONFIG_DIRECT_CHARGER
        is_scp_charger = 0;
#endif
        return;
    }

    check_temperature(di);
    interval = di->check_interval;
    hrtimer_start(&di->timer, ktime_set(interval/MSEC_PER_SEC, (interval % MSEC_PER_SEC) * USEC_PER_SEC), HRTIMER_MODE_REL);

}

static enum hrtimer_restart uscp_timer_func(struct hrtimer *timer)
{
    struct uscp_device_info *di;

    di = container_of(timer, struct uscp_device_info, timer);
    queue_work(di->uscp_wq, &di->uscp_check_wk);
    return HRTIMER_NORESTART;
}

static void check_ntc_error(void)
{
    int temp = 0;
    int sum = 0;
    int i = 0;
    int tbatt = INVALID_TEMP_VAL;
    int batt_id = 0;

    for (i = 0; i < GET_TEMP_VAL_NUM; ++i)
    {
        sum += get_temperature_value();
    }
    temp = sum / GET_TEMP_VAL_NUM;
    if (temp > CHECK_NTC_TEMP_MAX || temp < CHECK_NTC_TEMP_MIN)
    {
#ifdef CONFIG_HUAWEI_DSM
        if (!dsm_client_ocuppy(uscp_client))
        {
            tbatt = hisi_battery_temperature();
            batt_id = hisi_battery_id_voltage();
            dsm_client_record(uscp_client, "ntc error happened,tusb = %d,tbatt = %d,batt_id = %d\n",
                temp,tbatt,batt_id);
            dsm_client_notify(uscp_client, ERROR_NO_USB_SHORT_PROTECT_NTC);
        }
#endif
        protect_enable = 0;
    }
    else
    {
        protect_enable = 1;
    }
}

static int uscp_probe(struct platform_device *pdev)
{
    struct device_node* np;
    struct uscp_device_info* di;
    enum hisi_charger_type type = hisi_get_charger_type();
    int ret = 0;

    np = pdev->dev.of_node;
    if(NULL == np)
    {
        hwlog_err("np is NULL\n");
        return -1;
    }
    di = kzalloc(sizeof(*di), GFP_KERNEL);
    if (!di)
    {
        hwlog_err("di is NULL\n");
        return -ENOMEM;

    }
    g_di = di;
    platform_set_drvdata(pdev, di);

#ifdef CONFIG_HUAWEI_DSM
    if (!uscp_client)
    {
        uscp_client = dsm_register_client(&dsm_uscp);
    }
    if (NULL == uscp_client)
    {
        hwlog_err("uscp register dsm fail\n");
    }
#endif

    is_uscp_mode = 0;
    is_rt_uscp_mode = 0;
    di->keep_check_cnt = CHECK_CNT_INIT;

    di->gpio_uscp = of_get_named_gpio(np, "gpio_usb_short_circuit_protect",0);
    if (!gpio_is_valid(di->gpio_uscp))
    {
        hwlog_err("gpio_uscp is not valid\n");
        ret = -EINVAL;
        goto free_mem;
    }

    ret = gpio_request(di->gpio_uscp, "usb_short_circuit_protect");
    if (ret)
    {
        hwlog_err("could not request gpio_uscp\n");
        ret = -EINVAL;
        goto free_mem;
    }
    gpio_direction_output(di->gpio_uscp, 0);

    ret = of_property_read_u32(np, "adc_channel_uscp", &(di->adc_channel_uscp));
    if (ret)
    {
        hwlog_err("get adc_channel_uscp info fail!\n");
        ret = -EINVAL;
        goto free_gpio;
    }
    ret = of_property_read_u32(np, "uscp_threshold_tusb", &(di->uscp_threshold_tusb));
    if (ret)
    {
        di->uscp_threshold_tusb = DEFAULT_TUSB_THRESHOLD;
        hwlog_err("get uscp_threshold_tusb info fail!use default threshold = %d\n",di->uscp_threshold_tusb);
    }
    ret = of_property_read_u32(np, "open_mosfet_temp", &(di->open_mosfet_temp));
    if (ret)
    {
        hwlog_err("get open_mosfet_temp info fail!\n");
        ret = -EINVAL;
        goto free_gpio;
    }
    ret = of_property_read_u32(np, "open_hiz_temp", &(di->open_hiz_temp));
    if (ret)
    {
        di->open_hiz_temp = di->open_mosfet_temp;
        hwlog_err("get open_hiz_temp info fail,use default open_mosfet_temp!\n");
    }
    ret = of_property_read_u32(np, "dmd_hiz_enable", &(di->dmd_hiz_enable));
    if (ret)
    {
        di->dmd_hiz_enable = DMD_HIZ_DISABLE;
        hwlog_err("get dmd_hiz_enable info fail,use value zero!\n");
    }
    ret = of_property_read_u32(np, "close_mosfet_temp", &(di->close_mosfet_temp));
    if (ret)
    {
        hwlog_err("get close_mosfet_temp info fail!\n");
        ret = -EINVAL;
        goto free_gpio;
    }
    ret = of_property_read_u32(np, "interval_switch_temp", &(di->interval_switch_temp));
    if (ret)
    {
        hwlog_err("get interval_switch_temp info fail!\n");
        ret = -EINVAL;
        goto free_gpio;
    }
    check_ntc_error();
    if (!is_hisi_battery_exist()) {
        hwlog_err("battery is not exist, disable usb short protect!\n");
        protect_enable = 0;
    }
    if (!protect_enable)
    {
        goto free_gpio;
    }
    wake_lock_init(&uscp_wakelock, WAKE_LOCK_SUSPEND, "usb_short_circuit_protect_wakelock");
    di->uscp_wq = create_singlethread_workqueue("usb_short_circuit_protect_wq");
    INIT_WORK(&di->uscp_check_wk, uscp_check_work);
    hrtimer_init(&di->timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
    di->timer.function = uscp_timer_func;

#ifdef CONFIG_TCPC_CLASS
    if(is_pd_supported()) {
        di->tcpc_nb.notifier_call = pd_notifier_call;
        ret = register_pd_dpm_notifier(&di->tcpc_nb);
    } else {
#endif
        di->usb_nb.notifier_call = usb_notifier_call;
        ret = hisi_charger_type_notifier_register(&di->usb_nb);
#ifdef CONFIG_TCPC_CLASS
    }
#endif
    if (ret < 0)
    {
        hwlog_err("charger_type_notifier_register failed\n");
        ret = -EINVAL;
        goto free_gpio;
    }
    charge_type_handler(di, type);
    uscp_probe_ok = true;
    return 0;

free_gpio:
    gpio_free(di->gpio_uscp);
free_mem:
    platform_set_drvdata(pdev, NULL);
    kfree(di);
    g_di = NULL;
    return ret;
}

#ifdef CONFIG_PM
static int usb_short_circuit_protect_suspend(struct platform_device *pdev, pm_message_t state)
{
    struct uscp_device_info* di = platform_get_drvdata(pdev);
    if(NULL == di)
    {
        hwlog_err("%s:di is NULL\n", __func__);
        return 0;
    }
    cancel_work_sync(&di->uscp_check_wk);
    hrtimer_cancel(&di->timer);
    return 0;
}
static int usb_short_circuit_protect_resume(struct platform_device *pdev)
{
    struct uscp_device_info* di = platform_get_drvdata(pdev);
    if(NULL == di)
    {
        hwlog_err("%s:di is NULL\n", __func__);
        return 0;
    }
    enum hisi_charger_type type = hisi_get_charger_type();
    if( CHARGER_TYPE_NONE == type )
    {
        return 0;
    }
    queue_work(di->uscp_wq, &di->uscp_check_wk);
    return 0;
}
#endif

static struct of_device_id uscp_match_table[] =
{
    {
        .compatible = "huawei,usb_short_circuit_protect",
        .data = NULL,
    },
    {
    },
};
static struct platform_driver uscp_driver = {
    .probe = uscp_probe,
#ifdef CONFIG_PM
    /*depend on IPC driver,so we set SR suspend/resume and IPC is suspend_late/early_resume*/
    .suspend = usb_short_circuit_protect_suspend,
    .resume = usb_short_circuit_protect_resume,
#endif
    .driver = {
        .name = "huawei,usb_short_circuit_protect",
        .owner = THIS_MODULE,
        .of_match_table = of_match_ptr(uscp_match_table),
    },
};

static int __init uscp_init(void)
{
    return platform_driver_register(&uscp_driver);
}

device_initcall_sync(uscp_init);

static void __exit uscp_exit(void)
{
    platform_driver_unregister(&uscp_driver);
}

module_exit(uscp_exit);

MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:uscp");
MODULE_AUTHOR("HUAWEI Inc");
