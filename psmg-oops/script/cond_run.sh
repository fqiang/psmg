wait_name=psmg-oops

pid=`pgrep $wait_name`
while [ -d /proc/$pid ]; do
	echo "$wait_name ($pid)  is running -- sleep 10"
	sleep 10
	pid=`pgrep $wait_name`
done 

./run_prof_all alm_ssd.mod

