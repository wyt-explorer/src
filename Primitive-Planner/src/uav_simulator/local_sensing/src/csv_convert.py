"""文件作用：把 EuRoC 真值 CSV 转为时间、位置和四元数逐项换行的文本。
调用关系：离线运行后供 euroc.cpp 的位姿读取流程使用。
输入：脚本内固定路径的 data.csv；首列纳秒时间戳和后续七个位姿字段。
输出：同目录 data.txt，时间转换为秒，位置和四元数字段保持原值。
运行模式：离线数据预处理脚本。
主要依赖：Python csv、文件接口。
"""
import csv
file_location = '/home/wang/bag/banchmark/EuRoC/ViconRoom101/state_groundtruth_estimate0/data.csv'
with open('/home/wang/bag/banchmark/EuRoC/ViconRoom101/state_groundtruth_estimate0/data.txt', 'w') as txt_f:
	with open(file_location) as f:
	    f_csv = csv.reader(f)
	    headers = next(f_csv)
	    for row in f_csv:
	    	txt_f.write('%lf\n'% (float(row[0]) / 1000000000.0) )
	    	txt_f.write('%lf\n'% (float(row[1])) )
	    	txt_f.write('%lf\n'% (float(row[2])) )
	    	txt_f.write('%lf\n'% (float(row[3])) )
	    	txt_f.write('%lf\n'% (float(row[4])) )
	    	txt_f.write('%lf\n'% (float(row[5])) )
	    	txt_f.write('%lf\n'% (float(row[6])) )
	    	txt_f.write('%lf\n'% (float(row[7])) )