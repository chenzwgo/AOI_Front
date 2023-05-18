1.alarmcode.txt
  简介: 报警代码信息配置
  格式: 
 				 No,Code,Category,Desc
  			 行号,代码号,类别,简单描述(用于显示到界面上)
  
  例如:
  1,112,Material,Material Depleted

2.AlarmLog.csv
	简介:  报警历史log文件:
	格式:
				 Date,Time,Code,Category,Duration
			   日期,时间,代码号,类别,持续时间
	
	例如:
	10/1/14,22:11:47,112,Material,00:04:27
	10/1/14,16:45:18,143,Vision,02:15:23
	
3.modulecolor.txt
	简介:  饼状图模块颜色配置
	格式:
	       module,colorhex
	       模块(类别)，颜色16进制代码
	例如:
				 Material,#99e600
				 表示饼状图中 Material类别的颜色代码是 “#99e600”
