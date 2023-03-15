# Virus
ProjetFac-Sécurité is a college project where I needed to make a simple virus who is in front a image displayer but in the back get all the compiled C files (object files)
to rename actual object files by adding ".old" and then "MediaPlayer" duplicates himself with the actual object filename while images are displayed



# How it works
  On linux terminal you compile:  gcc -o MediaPlayer MediaPlayer.c `pkg-config --cflags --libs gtk+-3.0` -I/usr/include/gtk-3.0
  Then execute                 :  ./MediaPlayer
