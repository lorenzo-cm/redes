gnome-terminal -- bash -c "bin/server v4 51511; exec bash" &
sleep 1
gnome-terminal -- bash -c "bin/client 127.0.0.1 51511 -type temperature -coords 0 0; exec bash" &
sleep 1
gnome-terminal -- bash -c "bin/client 127.0.0.1 51511 -type temperature -coords 0 4; exec bash" &
sleep 1
gnome-terminal -- bash -c "bin/client 127.0.0.1 51511 -type temperature -coords 0 3; exec bash" &
sleep 1
gnome-terminal -- bash -c "bin/client 127.0.0.1 51511 -type temperature -coords 0 2; exec bash" &
sleep 1
gnome-terminal -- bash -c "bin/client 127.0.0.1 51511 -type temperature -coords 0 1; exec bash" &
