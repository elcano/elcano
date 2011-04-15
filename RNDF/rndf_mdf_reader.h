/*
 *Author : sherif ahmed
 *Date : 1/4/2011
 *Purpose : read rndf and mdf files
 */

#ifndef RNDF_MDF_READER_H
#define RNDF_MDF_READER_H

#include<stdlib.h>
#include <assert.h>
#include"parse_rndf_mdf.h"


struct id{
  int x;
  int y;
  int z;
};

struct waypoint{
  struct id *waypoint_id;
  double latitude;
  double longitude;
};

struct checkpoint{
  struct id *waypoint_id;
  int checkpoint_id;
};

struct stop{
  struct id *waypoint_id;
};

struct exit{
  struct id *exit_waypoint_id;
  struct id *entry_point_id;      //may be a waypoint or a perimeterpoint
};

struct lane{
  struct id *lane_id;
  int number_of_waypoints;
  int lane_width;

  char* left_boundary ;
  char* right_boundary;

  int number_of_checkpoints;
  struct checkpoint **checkpoints_list;

  int number_of_stops;
  struct stop **stops_list;

  int number_of_exits;
  struct exit **exits_list;

  struct waypoint **waypoints_list;
};

struct segment{
  int segment_id;
  int number_of_lanes;

  char *segment_name;

  struct lane **lanes_list;
};

struct perimeterpoint{
  struct id *perimeterpoint_id;
  double latitude;
  double longitude;
};

struct perimeter{
  struct id *perimeter_id;
  int number_of_perimeterpoints;

  int number_of_exits;
  struct exit **exits_list;

  struct perimeterpoint **perimeterpoints_list;
};

struct spot{
  struct id *spot_id;
  int spot_width;

  struct checkpoint *check_waypoint;
  
  struct waypoint *waypoint1;
  struct waypoint *waypoint2;
};

struct zone{
  int zone_id;
  int number_of_parking_spots;
  char *zone_name;
  
  struct perimeter *zone_perimeter;

  struct spot **parking_spots_list;
};

struct RNDF{
  char *RNDF_name;
  int number_of_segments;
  int number_of_zones;
  char *format_version;
  char *creation_date;

  struct segment **segments_list;
  struct zone **zones_list;
};

struct speed_limit{
  int id;
  int min_speed;
  int max_speed;
};

struct MDF{
  char *MDF_name;
  char *RNDF_name;
  char *format_version;
  char *creation_date;
  int number_of_checkpoints;
  int *checkpoints_list;
  int number_of_speed_limits;
  struct speed_limit **speed_limits_list;
};

struct RNDF *parseAnalyzeRndfFile(FILE*);
struct MDF *parseAnalyzeMdfFile(FILE*);
void freeRNDF(struct RNDF*);
void freeMDF(struct MDF*);

#endif
