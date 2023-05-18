#ifndef IO_DEFINE_H
#define IO_DEFINE_H

#define DI_RESET_SIGNAL "di_reset"
#define DI_PAUSE_SIGNAL "di_pause"
#define DI_START_SIGNAL "di_start"
#define DI_ESTOP_SIGNAL "di_estop"
#define DI_IN_SIG       "di_in_sig"        //入料口产品信号光纤//
#define DI_OUT_END_SIG   "di_out_end_sig"   //出料口产品信号光纤//
#define DI_SNAP_SIG_0    "di_snap_sig_0"
#define DI_SNAP_SIG_1    "di_snap_sig_1"
#define DI_SNAP_SIG_2    "di_snap_sig_2"
#define DI_SNAP_SIG_3    "di_snap_sig_3"
#define DI_CLEAN_RUN     "di_clean_run"        // 清洗机运行
#define DI_SHANG_RUN     "di_shang_run"        // 上料机运行
#define DI_XIA_RUN       "di_xiao_run"    //下料机运行
#define DI_XIA_MANLIAO       "di_xiao_manliao"    //下料机运行
#define DI_CLEAR_MODE    "di_clear_mode"    // 检测到下料机给的清料模式
#define DI_PENMA_SIG    "di_penma_sig"     // 喷码输入信号
#define DI_FULL_SIG      "di_full_sig"      //下料机满载信号


#define DO_OUT_NG               "do_out_red"       //输出NG给下料机
#define DO_OUT_RI               "do_out_yellow"    //输出RI给下料机
#define DO_OUT_OK               "do_out_green"    //输出OK给下料机

#define DO_IN_LIGHT                 "do_in_light"    //入料指示灯
#define DO_ELEC_LOCK                "do_elec_lock"          //电磁锁
#define DO_GREED_LIGHT              "do_green"              // 三色灯
#define DO_YELLOW_LIGHT             "do_yellow"             // 三色灯
#define DO_RED_LIGHT                "do_red"                // 三色灯
#define DO_ELEC_LOCK                "do_elec_lock"          //电磁锁
#define DO_FFU                      "do_ffu"          //FFU
#define DO_BEE_ALARM                "do_bee_alarm"      // 蜂鸣器

#define DO_CLEAN_PAUSE      "do_clean_pause"   // 清洗机暂停
#define DO_CLEAN_RUN        "do_clean_run"     // 清洗机启动
#define DO_SHANG_STOP       "do_shang_stop"    // 上料机停止
#define DO_SHANG_START       "do_shang_start"    // 上料机启动
#define DO_XIA_STOP          "do_xia_stop"      //下料机停止
#define DO_PENMA_OUT        "do_pen_out"      //下料机运行

#define DO_OTHER_AOI_STOP        "do_other_aoi_stop"      //对线aoi停轴运动



#endif //IO_DEFINE_H														          
																		         
																		
																		
