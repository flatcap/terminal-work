while :; do xlsclients  | grep -q terminal; if [ $? = 0 ]; then title TERMINAL; else title -; fi; usleep 500000; done
