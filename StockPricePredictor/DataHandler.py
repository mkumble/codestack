def getData(ls_symbols,dt_start,dt_end):
	dt_timeofday = dt.timedelta(hours=16)
	ldt_timestamps = du.getNYSEdays(dt_start, dt_end, dt_timeofday)
	#os.environ['QSDATA']="/home/mkumble3/QSTK-0.2.8/QSTK/QSData"
	dataobj = da.DataAccess('Yahoo')
	ls_keys = ['actual_close']
	ldf_data = dataobj.get_data(ldt_timestamps, ls_symbols, ls_keys)
	d_data = dict(zip(ls_keys, ldf_data))
	return d_data
