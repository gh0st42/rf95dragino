# Makefile
# Sample for RH_RF95 (client and server) on Raspberry Pi
# Caution: requires bcm2835 library to be already installed
# http://www.airspayce.com/mikem/bcm2835/

CC            = g++
CFLAGS        = -DRASPBERRY_PI -DBCM2835_NO_DELAY_COMPATIBILITY -D__BASEFILE__=\"$*\" -DDESKTOP -DRFM95_CS=22 -DRFM95_INT=7 -DRFM95_RST=11 -DBOARD_DRAGINO_PIHAT -DUSE_GPS
LIBS          = -lbcm2835 -lgps
RADIOHEADBASE = lib/radiohead
RF95MODEMBASE = lib/rf95modem/src
INCLUDE       = -I$(RADIOHEADBASE) -I$(RADIOHEADBASE)/examples/raspi -Ilib/piduino_stripped -I$(RF95MODEMBASE) -Isrc

all: rf95modem

RasPi.o: $(RADIOHEADBASE)/RHutil/RasPi.cpp
				$(CC) $(CFLAGS) -c $(RADIOHEADBASE)/RHutil/RasPi.cpp $(INCLUDE)

gps.o: src/gps.cpp
				$(CC) $(CFLAGS) -c $(INCLUDE) $<

main.o: src/main.cpp
				$(CC) $(CFLAGS) -c $(INCLUDE) $<

piduino_string.o: lib/piduino_stripped/piduino_string.cpp
				$(CC) $(CFLAGS) -c $(INCLUDE) $<

RH_RF95.o: $(RADIOHEADBASE)/RH_RF95.cpp
				$(CC) $(CFLAGS) -c $(INCLUDE) $<

RHDatagram.o: $(RADIOHEADBASE)/RHDatagram.cpp
				$(CC) $(CFLAGS) -c $(INCLUDE) $<

RHHardwareSPI.o: $(RADIOHEADBASE)/RHHardwareSPI.cpp
				$(CC) $(CFLAGS) -c $(INCLUDE) $<

RHSPIDriver.o: $(RADIOHEADBASE)/RHSPIDriver.cpp
				$(CC) $(CFLAGS) -c $(INCLUDE) $<

RHGenericDriver.o: $(RADIOHEADBASE)/RHGenericDriver.cpp
				$(CC) $(CFLAGS) -c $(INCLUDE) $<

RHGenericSPI.o: $(RADIOHEADBASE)/RHGenericSPI.cpp
				$(CC) $(CFLAGS) -c $(INCLUDE) $<

modem.o: $(RF95MODEMBASE)/modem.cpp
				$(CC) $(CFLAGS) -c $(INCLUDE) $<

rf95modem: 		main.o         RH_RF95.o RasPi.o RHHardwareSPI.o RHGenericDriver.o RHGenericSPI.o RHSPIDriver.o piduino_string.o modem.o gps.o
				$(CC) $^ $(LIBS) -o $@

clean:
				rm -rf *.o rf95modem
