// Copyright (c) 2018 Lars Baumgaertner

#ifdef USE_GPS
#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <libgpsmm.h>

#include "modem.h"
#include "gps.h"

gpsmm gps_rec("localhost", DEFAULT_GPSD_PORT);

void init_gps()
{
    printf("Init GPS subsystem.\n");
    if (gps_rec.stream(WATCH_ENABLE | WATCH_JSON) == NULL)
    {
        std::cerr << "No GPSD running.\n";
        return;
    }
}

void power_gps(bool powerup)
{
    if (powerup)
    {
        // TODO: unimplemented
    }
    else
    {
        // TODO: unimplemented
    }
}
bool gps_enabled()
{
    return true;
}
static void libgps_dump_state(struct gps_data_t *collect)
{
    /* no need to dump the entire state, this is a sanity check */
#ifndef USE_QT
    /* will fail on a 32-bit macine */
    (void)fprintf(stdout, "flags: (0x%04x) %s\n",
                  (unsigned int)collect->set, gps_maskdump(collect->set));
#endif
    if (collect->set & ONLINE_SET)
        (void)fprintf(stdout, "ONLINE: %lf\n", collect->online);
    if (collect->set & TIME_SET)
        (void)fprintf(stdout, "TIME: %lf\n", collect->fix.time);
    if (collect->set & LATLON_SET)
        (void)fprintf(stdout, "LATLON: lat/lon: %lf %lf\n",
                      collect->fix.latitude, collect->fix.longitude);
    if (collect->set & ALTITUDE_SET)
        (void)fprintf(stdout, "ALTITUDE: altitude: %lf  U: climb: %lf\n",
                      collect->fix.altitude, collect->fix.climb);
    if (collect->set & SPEED_SET)
        (void)fprintf(stdout, "SPEED: %lf\n", collect->fix.speed);
    if (collect->set & TRACK_SET)
        (void)fprintf(stdout, "TRACK: track: %lf\n", collect->fix.track);
    if (collect->set & CLIMB_SET)
        (void)fprintf(stdout, "CLIMB: climb: %lf\n", collect->fix.climb);
    if (collect->set & STATUS_SET)
        (void)fprintf(stdout, "STATUS: status: %d\n", collect->status);
    if (collect->set & MODE_SET)
        (void)fprintf(stdout, "MODE: mode: %d\n", collect->fix.mode);
    if (collect->set & DOP_SET)
        (void)fprintf(stdout,
                      "DOP: satellites %d, pdop=%lf, hdop=%lf, vdop=%lf\n",
                      collect->satellites_used, collect->dop.pdop,
                      collect->dop.hdop, collect->dop.vdop);
    if (collect->set & VERSION_SET)
        (void)fprintf(stdout, "VERSION: release=%s rev=%s proto=%d.%d\n",
                      collect->version.release,
                      collect->version.rev,
                      collect->version.proto_major,
                      collect->version.proto_minor);
    if (collect->set & POLICY_SET)
        (void)fprintf(stdout,
                      "POLICY: watcher=%s nmea=%s raw=%d scaled=%s timing=%s, devpath=%s\n",
                      collect->policy.watcher ? "true" : "false",
                      collect->policy.nmea ? "true" : "false",
                      collect->policy.raw,
                      collect->policy.scaled ? "true" : "false",
                      collect->policy.timing ? "true" : "false",
                      collect->policy.devpath);
    if (collect->set & SATELLITE_SET)
    {
        int i;

        (void)fprintf(stdout, "SKY: satellites in view: %d\n",
                      collect->satellites_visible);
        /*for (i = 0; i < collect->satellites_visible; i++)
        {
            (void)fprintf(stdout, "    %2.2d: %2.2d %3.3d %3.0f %c\n",
                          collect->PRN[i], collect->elevation[i],
                          collect->azimuth[i], collect->ss[i],
                          collect->used[i] ? 'Y' : 'N');
        }*/
    }
    if (collect->set & DEVICE_SET)
        (void)fprintf(stdout, "DEVICE: Device is '%s', driver is '%s'\n",
                      collect->dev.path, collect->dev.driver);
#ifdef OLDSTYLE_ENABLE
    if (collect->set & DEVICEID_SET)
        (void)fprintf(stdout, "GPSD ID is %s\n", collect->dev.subtype);
#endif /* OLDSTYLE_ENABLE */
    if (collect->set & DEVICELIST_SET)
    {
        int i;
        (void)fprintf(stdout, "DEVICELIST:%d devices:\n",
                      collect->devices.ndevices);
        for (i = 0; i < collect->devices.ndevices; i++)
        {
            (void)fprintf(stdout, "%d: path='%s' driver='%s'\n",
                          collect->devices.ndevices,
                          collect->devices.list[i].path,
                          collect->devices.list[i].driver);
        }
    }
}

void print_gps()
{
    if (!gps_rec.waiting(1000000))
    {
        std::cerr << "+FAIL: GPS not ready.\n";
        return;
    }

    struct gps_data_t *gpsd_data;
    while (((gpsd_data = gps_rec.read()) == NULL) ||
           (gpsd_data->fix.mode < MODE_2D))
    {
        // Do nothing until fix
    }

    timestamp_t ts{gpsd_data->fix.time};
    //auto latitude{gpsd_data->fix.latitude};
    //auto longitude{gpsd_data->fix.longitude};

    //libgps_dump_state(gpsd_data);
    printf("Latitude  : %lf\n", gpsd_data->fix.latitude);
    printf("Longitude : %lf\n", gpsd_data->fix.longitude);
    printf("Satellites: %d\n", gpsd_data->satellites_visible);
    printf("Altitude  : %.02lfM\n", gpsd_data->fix.altitude / 3.2808);
    printf("Timestamp : %lf\n", gpsd_data->fix.time);
    // convert GPSD's timestamp_t into time_t
    time_t seconds{(time_t)ts};
    auto tm = *std::localtime(&seconds);

    std::ostringstream oss;
    //oss << std::put_time(&tm, "%d-%m-%Y %H:%M:%S");
    oss << std::put_time(&tm, "%H:%M:%S");
    //auto time_str{oss.str()};
    printf("Time      : %s\n", oss.str().c_str());

    out_println("+OK");
}

void gps_loop_tick()
{

    //if (GPS1.available())
    //    gps.encode(GPS1.read());
}
#endif