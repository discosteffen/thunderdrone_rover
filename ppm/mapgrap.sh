while true
do 
    rosrun map_server map_saver -f map
    convert map.pgm map.png
    sleep 1
done
