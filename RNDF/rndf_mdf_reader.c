/*
 *Author : sherif ahmed
 *Date : 1/4/2011
 *Purpose : read rndf and mdf files
 */

#include"rndf_mdf_reader.h"

#define MAX_STRING_LENGTH 128
#define MAX_INTEGER 32768

struct id *createId(char*);
struct waypoint *createWaypoint(char*);
struct checkpoint *createCheckpoint(char*);
struct stop *createStop(char*);
struct exit *createExit(char*);
struct lane *createLane(char**);
struct segment *createSegment(char**);
struct perimeterpoint *createPerimeterpoint(char*);
struct perimeter *createPerimeter(char**);
struct spot *createSpot(char**);
struct zone *createZone(char**);
struct RNDF *createRNDF(char**);
struct speed_limit *createSpeed_limit(char*);
struct MDF *createMDF(char**);
void freeId(struct id*);
void freeWaypoint(struct waypoint*);
void freeCheckpoint(struct checkpoint*);
void freeStop(struct stop*);
void freeExit(struct exit*);
void freeLane(struct lane*);
void freeSegment(struct segment*);
void freePerimeterpoint(struct perimeterpoint*);
void freePerimeter(struct perimeter*);
void freeSpot(struct spot*);
void freeZone(struct zone*);
void freeSpeed_limit(struct speed_limit*);


/*
 *createId function
 *
 *l : a pointer to astring represinting an id like : x.y.z or x.y
 *
 *this function takes a pointer to a string that represent an id and return
 *a struct id, if the id was only x.y , z will be -1.
 */
struct id *createId(char *l)
{
  struct id *i;
  char temp[6];
  char c;
  int n1 = 0;
  int n2 = 0;
  int n3 = 0;
  
  i = malloc(sizeof(struct id));
  assert(i != 0);
  while((c = l[n1++]) != '\0'){
    if(c != '.'){
      temp[n2] = c;
      n2++;
    }
    else if(c == '.'){
      temp[n2] = '\0';
      assert(n3 < 2);
      if(n3 == 0)
	i -> x = atoi(temp);

      else if(n3 ==1)
	i -> y = atoi(temp);
      
      n3++;
      n2 = 0;
    }
  }
  assert(n3 == 2 || n3 == 1);
  temp[n2] = '\0';
    
  if(n3 == 1){
    i -> y = atoi(temp);
    i -> z = -1;
  }
  else
    i -> z = atoi(temp);

  return i;
}

/*
 *createWaypoint function
 *
 *line : a pointer to a string represinting a waypoint
 *
 *this function will take a string represinting a waypoint
 *and return a struct waypoint
 */
struct waypoint *createWaypoint(char *line)
{
  int now;
  char **params;
  struct waypoint *wp;
  
  wp = malloc(sizeof(struct waypoint));
  assert(wp != 0);
  now = numberOfWords(line);
  assert(now == 3);
  params = divideLine(line, now, MAX_STRING_LENGTH);
  assert(params != 0);
  wp -> waypoint_id = createId(params[0]);
  wp -> latitude = atof(params[1]);
  wp -> longitude = atof(params[2]);
  freeCharArray(params, now);

  return wp; 
}

/*
 *createCheckpoint function
 *
 *line : a pointer to a string represinting a checkpoint
 *
 *this function will take a string represinting a checkpoint
 *and return a struct checkpoint
 */
struct checkpoint *createCheckpoint(char *line)
{
  char **params;
  struct checkpoint *cp;
  int now;

  cp = malloc(sizeof(struct checkpoint));
  assert(cp != 0);
  now = numberOfWords(line);
  assert(now == 3);
  params = divideLine(line, now, MAX_STRING_LENGTH);
  assert(params != 0);
  cp -> waypoint_id = createId(params[1]);
  cp -> checkpoint_id = atoi(params[2]);
  freeCharArray(params, now);

  return cp;
}

/*
 *createStop function
 *
 *line : a pointer to a string represinting a stop
 *
 *this function will take a string represinting a stop
 *and return a struct stop
 */
struct stop *createStop(char *line)
{
  char **params;
  struct stop *s;
  int now;

  s = malloc(sizeof(struct stop));
  assert(s != 0);
  now = numberOfWords(line);
  assert(now == 2);
  params = divideLine(line, now, MAX_STRING_LENGTH);
  assert(params != 0);
  s -> waypoint_id = createId(params[1]);
  freeCharArray(params, now);

  return s;
}

/*
 *createExit function
 *
 *line : a pointer to a string represinting an exit
 *
 *this function will take a string represinting an exit
 *and return a struct exit
 */
struct exit *createExit(char *line)
{
  char **params;
  struct exit *e;
  int now;

  e = malloc(sizeof(struct exit));
  assert(e != 0);
  now = numberOfWords(line);
  assert(now == 3);
  params = divideLine(line, now, MAX_STRING_LENGTH);
  assert(params != 0);
  e -> exit_waypoint_id = createId(params[1]);
  e -> entry_point_id = createId(params[2]);
  freeCharArray(params, now);

  return e;
}

/*
 *createLane function
 *
 *p : a pointer to an array of strings represinting a lane
 *
 *this function will take an array of strings represinting a lane
 *and return a struct lane
 */
struct lane *createLane(char **p)
{
  struct lane *l;
  char *temp;
  char **params;
  int now;
  int n = 0;
  int i;
  int numOfCkPts;
  int numOfSps;
  int numOfEts;

  l = malloc(sizeof(struct lane));
  assert( l != 0);
  now = numberOfWords(p[n]);
  assert(now == 2);
  params = divideLine(p[n], now, MAX_STRING_LENGTH);
  assert(params != 0);
  l -> lane_id = createId(params[1]);
  freeCharArray(params, now);
  n++;
  
  now = numberOfWords(p[n]);
  assert(now == 2);
  params = divideLine(p[n], now, MAX_STRING_LENGTH);
  assert(params != 0);
  l -> number_of_waypoints = atoi(params[1]);
  freeCharArray(params, now);
  n++;
  
  temp = "lane_width";
  now = numberOfWords(p[n]);
  params = divideLine(p[n], now, MAX_STRING_LENGTH);
  assert(params != 0);
  if(compTwoStrings(params[0], temp)){
    assert(now == 2);
    l -> lane_width = atoi(params[1]);
    n++;
  }
  else
    l -> lane_width = -1;
  freeCharArray(params, now);

  temp = "left_boundary";
  now = numberOfWords(p[n]);
  params = divideLine(p[n], now, MAX_STRING_LENGTH);
  assert(params != 0);
  if(compTwoStrings(params[0], temp)){
    assert(now == 2);
    l -> left_boundary = malloc(MAX_STRING_LENGTH * sizeof(char));
    stringCopy(params[1],l -> left_boundary);
    n++;
  }
  else
    l -> left_boundary = 0;
  freeCharArray(params, now);
  
  temp = "right_boundary";
  now = numberOfWords(p[n]);
  params = divideLine(p[n], now, MAX_STRING_LENGTH);
  assert(params != 0);
  if(compTwoStrings(params[0], temp)){
    assert(now == 2);
    l -> right_boundary = malloc(MAX_STRING_LENGTH * sizeof(char));
    stringCopy(params[1],l -> right_boundary);
    n++;
  }
  else
    l -> right_boundary = 0;
  freeCharArray(params, now);

  numOfCkPts = 0;
  temp = "checkpoint";
  now = numberOfWords(p[n]);
  params = divideLine(p[n], now, MAX_STRING_LENGTH);
  assert(params != 0);
  while(compTwoStrings(params[0], temp)){
    assert(now == 3);
    freeCharArray(params, now);
    numOfCkPts++;
    now = numberOfWords(p[n + numOfCkPts]);
    params = divideLine(p[n + numOfCkPts], now, MAX_STRING_LENGTH);
    assert(params != 0);
  }
  freeCharArray(params, now);
  l -> number_of_checkpoints = numOfCkPts;
  if(numOfCkPts > 0)
    l -> checkpoints_list = malloc(numOfCkPts * sizeof(struct checkpoint));
  else
    l -> checkpoints_list = 0;
  for(i = 0; i < numOfCkPts; i++){
    l -> checkpoints_list[i] = createCheckpoint(p[n]);
    n++;
  }

  numOfSps = 0;
  temp = "stop";
  now = numberOfWords(p[n]);
  params = divideLine(p[n], now, MAX_STRING_LENGTH);
  assert(params != 0);
  while(compTwoStrings(params[0], temp)){
    assert(now == 2);
    freeCharArray(params, now);
    numOfSps++;
    now = numberOfWords(p[n + numOfSps]);
    params = divideLine(p[n + numOfSps], now, MAX_STRING_LENGTH);
    assert(params != 0);
  }
  freeCharArray(params, now);
  l -> number_of_stops = numOfSps;
  if(numOfSps > 0)
    l -> stops_list = malloc(numOfSps * sizeof(struct stop));
  else
    l -> stops_list = 0;
  for(i = 0; i < numOfSps; i++){
    l -> stops_list[i] = createStop(p[n]);
    n++;
  }

  numOfEts = 0;
  temp = "exit";
  now = numberOfWords(p[n]);
  params = divideLine(p[n], now, MAX_STRING_LENGTH);
  assert(params != 0);
  while(compTwoStrings(params[0], temp)){
    assert(now == 3);
    freeCharArray(params, now);
    numOfEts++;
    now = numberOfWords(p[n + numOfEts]);
    params = divideLine(p[n + numOfEts], now, MAX_STRING_LENGTH);
    assert(params != 0);
  }
  freeCharArray(params, now);
  l -> number_of_exits = numOfEts;
  if(numOfEts > 0)
    l -> exits_list = malloc(numOfEts * sizeof(struct stop));
  else
    l -> exits_list = 0;
  for(i = 0; i < numOfEts; i++){
    l -> exits_list[i] = createExit(p[n]);
    n++;
  }

  l -> waypoints_list = malloc(
			       (l -> number_of_waypoints) * 
			       sizeof(struct waypoint));
  assert((l -> waypoints_list) != 0);
  for(i = 0; i < l -> number_of_waypoints; i++){
    l -> waypoints_list[i] = createWaypoint(p[n]);
    n++;
  }

  return l;
}

/*
 *createSegment function
 *
 *p : a pointer to an array of strings represinting a segment
 *
 *this function will take an array of strings represinting a segment
 *and return a struct segment
 */
struct segment *createSegment(char **p)
{
  struct segment *s;
  char *temp;
  char **params;
  int now;
  int n = 0;
  int i;
  int lane_start;
  int lane_end;

  s = malloc(sizeof(struct segment));
  assert(s != 0);
  now = numberOfWords(p[n]);
  assert(now == 2);
  params = divideLine(p[n], now, MAX_STRING_LENGTH);
  assert(params != 0);
  s -> segment_id = atoi(params[1]);
  freeCharArray(params, now);
  n++;
  
  now = numberOfWords(p[n]);
  assert(now == 2);
  params = divideLine(p[n], now, MAX_STRING_LENGTH);
  assert(params != 0);
  s -> number_of_lanes = atoi(params[1]);
  freeCharArray(params, now);
  n++;
  
  temp = "segment_name";
  now = numberOfWords(p[n]);
  params = divideLine(p[n], now, MAX_STRING_LENGTH);
  assert(params != 0);
  if(compTwoStrings(params[0], temp)){
    assert(now == 2);
    s -> segment_name = malloc(MAX_STRING_LENGTH * sizeof(char));
    stringCopy(params[1], s -> segment_name);
    n++;
  }
  else
    s -> segment_name = 0;
  freeCharArray(params, now);

  s -> lanes_list = malloc(s -> number_of_lanes * sizeof(struct lane));
  lane_start = n;
  lane_end = n + 1;
  temp = "end_lane";
  for(i = 0; i < s -> number_of_lanes; i++){
    s -> lanes_list[i] = createLane((p + lane_start));
    now = numberOfWords(p[lane_end]);
    params = divideLine(p[lane_end], now, MAX_STRING_LENGTH);
    assert(params != 0);
    while(!compTwoStrings(temp, params[0])){
      freeCharArray(params, now);
      lane_end++;
      now = numberOfWords(p[lane_end]);
      params = divideLine(p[lane_end], now, MAX_STRING_LENGTH);
      assert(params != 0);
    }
    freeCharArray(params, now);
    lane_end++;
    lane_start = lane_end;
  }
  return s;
}

/*
 *createPerimeterpoint function
 *
 *line : a pointer to a string represinting a Perimeterpoint
 *
 *this function will take a string represinting a Perimeterpoint
 *and return a struct Perimeterpoint
 */
struct perimeterpoint *createPerimeterpoint(char *line)
{
  int now;
  char **params;
  struct perimeterpoint *pp;
  
  pp = malloc(sizeof(struct perimeterpoint));
  assert(pp != 0);
  now = numberOfWords(line);
  assert(now == 3);
  params = divideLine(line, now, MAX_STRING_LENGTH);
  assert(params != 0);
  pp -> perimeterpoint_id = createId(params[0]);
  pp -> latitude = atof(params[1]);
  pp -> longitude = atof(params[2]);
  freeCharArray(params, now);

  return pp; 
}

/*
 *createPerimeter function
 *
 *p : a pointer to an array of strings represinting a perimeter
 *
 *this function will take an array of strings represinting a perimeter
 *and return a struct perimeter
 */
struct perimeter *createPerimeter(char **p)
{
  struct perimeter *p_s;
  char *temp;
  char **params;
  int now;
  int n = 0;
  int i;
  int numOfEts;

  p_s = malloc(sizeof(struct perimeter));
  assert(p_s != 0);
  now = numberOfWords(p[n]);
  assert(now == 2);
  params = divideLine(p[n], now, MAX_STRING_LENGTH);
  assert(params != 0);
  p_s -> perimeter_id = createId(params[1]);
  freeCharArray(params, now);
  n++;
  
  now = numberOfWords(p[n]);
  assert(now == 2);
  params = divideLine(p[n], now, MAX_STRING_LENGTH);
  assert(params != 0);
  p_s -> number_of_perimeterpoints = atoi(params[1]);
  freeCharArray(params, now);
  n++;
  
  numOfEts = 0;
  temp = "exit";
  now = numberOfWords(p[n]);
  params = divideLine(p[n], now, MAX_STRING_LENGTH);
  assert(params != 0);
  while(compTwoStrings(params[0], temp)){
    assert(now == 3);
    freeCharArray(params, now);
    numOfEts++;
    now = numberOfWords(p[n + numOfEts]);
    params = divideLine(p[n + numOfEts], now, MAX_STRING_LENGTH);
    assert(params != 0);
  }
  freeCharArray(params, now);
  p_s -> number_of_exits = numOfEts;
  if(numOfEts > 0)
    p_s -> exits_list = malloc(numOfEts * sizeof(struct stop));
  else
    p_s -> exits_list = 0;
  for(i = 0; i < numOfEts; i++){
    p_s -> exits_list[i] = createExit(p[n]);
    n++;
  }

  p_s -> perimeterpoints_list = malloc(
			       (p_s -> number_of_perimeterpoints) * 
			       sizeof(struct perimeterpoint));
  assert((p_s -> perimeterpoints_list) != 0);
  for(i = 0; i < p_s -> number_of_perimeterpoints; i++){
    p_s -> perimeterpoints_list[i] = createPerimeterpoint(p[n]);
    n++;
  }
  return p_s;
}

/*
 *createSpot function
 *
 *p : a pointer to an array of strings represinting a spot
 *
 *this function will take an array of strings represinting a spot
 *and return a struct spot
 */
struct spot *createSpot(char **p)
{
  struct spot *s_s;
  char *temp;
  char **params;
  int now;
  int n = 0;
  
  s_s = malloc(sizeof(struct spot));
  assert(s_s != 0);
  now = numberOfWords(p[n]);
  assert(now == 2);
  params = divideLine(p[n], now, MAX_STRING_LENGTH);
  assert(params != 0);
  s_s -> spot_id = createId(params[1]);
  freeCharArray(params, now);
  n++;
  
  temp = "spot_width";
  now = numberOfWords(p[n]);
  params = divideLine(p[n], now, MAX_STRING_LENGTH);
  assert(params != 0);
  if(compTwoStrings(params[0], temp)){
    assert(now == 2);
    s_s -> spot_width = atoi(params[1]);
    n++;
  }
  else
    s_s -> spot_width = -1;
  freeCharArray(params, now);

  temp = "checkpoint";
  now = numberOfWords(p[n]);
  params = divideLine(p[n], now, MAX_STRING_LENGTH);
  assert(params != 0);
  if(compTwoStrings(params[0], temp)){
    assert(now == 3);
    s_s -> check_waypoint = createCheckpoint(p[n]);
    n++;
  }
  else
    s_s -> check_waypoint = 0;
  freeCharArray(params, now);

  s_s -> waypoint1= createWaypoint(p[n++]);
  s_s -> waypoint2= createWaypoint(p[n]);

  return s_s;
}

/*
 *createZone function
 *
 *p : a pointer to an array of strings represinting a zone
 *
 *this function will take an array of strings represinting a zone
 *and return a struct zone
 */
struct zone *createZone(char **p)
{
  struct zone *z;
  char *temp;
  char **params;
  int now;
  int n = 0;
  int i;
  int end_of_perimeter;
  int s_start;
  int s_end;

  z = malloc(sizeof(struct zone));
  assert(z != 0);
  now = numberOfWords(p[n]);
  assert(now == 2);
  params = divideLine(p[n], now, MAX_STRING_LENGTH);
  assert(params != 0);
  z -> zone_id = atoi(params[1]);
  freeCharArray(params, now);
  n++;
  
  now = numberOfWords(p[n]);
  assert(now == 2);
  params = divideLine(p[n], now, MAX_STRING_LENGTH);
  assert(params != 0);
  z -> number_of_parking_spots = atoi(params[1]);
  freeCharArray(params, now);
  n++;

  temp = "zone_name";
  now = numberOfWords(p[n]);
  params = divideLine(p[n], now, MAX_STRING_LENGTH);
  assert(params != 0);
  if(compTwoStrings(params[0], temp)){
    assert(now == 2);
    z -> zone_name = malloc(MAX_STRING_LENGTH * sizeof(char));
    stringCopy(params[1], z -> zone_name);
    n++;
  }
  else
    z -> zone_name = 0;
  freeCharArray(params, now);

  z -> zone_perimeter = createPerimeter(p + n);
  end_of_perimeter = n;
  temp = "end_perimeter";
  now = numberOfWords(p[end_of_perimeter]);
  params = divideLine(p[end_of_perimeter], now, MAX_STRING_LENGTH);
  assert(params != 0);
  while(!compTwoStrings(temp, params[0])){
    freeCharArray(params, now);
    end_of_perimeter++;
    now = numberOfWords(p[end_of_perimeter]);
    params = divideLine(p[end_of_perimeter], now, MAX_STRING_LENGTH);
    assert(params != 0);
  }
  freeCharArray(params, now);
  n = ++end_of_perimeter;

  z -> parking_spots_list = 
    malloc(z -> number_of_parking_spots * sizeof(struct zone));
  s_start = n;
  s_end = n + 1;
  temp = "end_spot";
  for(i = 0; i < z -> number_of_parking_spots; i++){
    z -> parking_spots_list[i] = createSpot((p + s_start));
    now = numberOfWords(p[s_end]);
    params = divideLine(p[s_end], now, MAX_STRING_LENGTH);
    assert(params != 0);
    while(!compTwoStrings(temp, params[0])){
      freeCharArray(params, now);
      s_end++;
      now = numberOfWords(p[s_end]);
      params = divideLine(p[s_end], now, MAX_STRING_LENGTH);
      assert(params != 0);
    }
    freeCharArray(params, now);
    s_end++;
    s_start = s_end;
  }
  return z;
}

/*
 *createRNDF function
 *
 *p : a pointer to an array of strings represinting a RNDF
 *
 *this function will take an array of strings represinting a RNDF
 *and return a struct RNDF
 */
struct RNDF *createRNDF(char **p)
{
  struct RNDF *r;
  char *temp;
  char **params;
  int now;
  int n = 0;
  int i;
  int s_start;
  int s_end;
  int z_start;
  int z_end;

  r = malloc(sizeof(struct RNDF));
  assert(r != 0);
  now = numberOfWords(p[n]);
  assert(now == 2);
  params = divideLine(p[n], now, MAX_STRING_LENGTH);
  assert(params != 0);
  r -> RNDF_name = malloc(MAX_STRING_LENGTH * sizeof(char));
  stringCopy(params[1], r -> RNDF_name);
  freeCharArray(params, now);
  n++;

  now = numberOfWords(p[n]);
  assert(now == 2);
  params = divideLine(p[n], now, MAX_STRING_LENGTH);
  assert(params != 0);
  r -> number_of_segments = atoi(params[1]);
  freeCharArray(params, now);
  n++;

  now = numberOfWords(p[n]);
  assert(now == 2);
  params = divideLine(p[n], now, MAX_STRING_LENGTH);
  assert(params != 0);
  r -> number_of_zones = atoi(params[1]);
  freeCharArray(params, now);
  n++;

  temp = "format_version";
  now = numberOfWords(p[n]);
  params = divideLine(p[n], now, MAX_STRING_LENGTH);
  assert(params != 0);
  if(compTwoStrings(params[0], temp)){
    assert(now == 2);
    r -> format_version = malloc(MAX_STRING_LENGTH * sizeof(char));
    stringCopy(params[1], r -> format_version);
    n++;
  }
  else
    r -> format_version = 0;
  freeCharArray(params, now);

  temp = "creation_date";
  now = numberOfWords(p[n]);
  params = divideLine(p[n], now, MAX_STRING_LENGTH);
  assert(params != 0);
  if(compTwoStrings(params[0], temp)){
    assert(now == 2);
    r ->  creation_date = malloc(MAX_STRING_LENGTH * sizeof(char));
    stringCopy(params[1], r ->  creation_date);
    n++;
  }
  else
    r ->  creation_date = 0;
  freeCharArray(params, now);

  r -> segments_list = 
    malloc(r -> number_of_segments * sizeof(struct segment));
  s_start = n;
  s_end = n + 1;
  temp = "end_segment";
  for(i = 0; i < r -> number_of_segments; i++){
    r -> segments_list[i] = createSegment((p + s_start));
    now = numberOfWords(p[s_end]);
    params = divideLine(p[s_end], now, MAX_STRING_LENGTH);
    assert(params != 0);
    while(!compTwoStrings(temp, params[0])){
      freeCharArray(params, now);
      s_end++;
      now = numberOfWords(p[s_end]);
      params = divideLine(p[s_end], now, MAX_STRING_LENGTH);
      assert(params != 0);
    }
    freeCharArray(params, now);
    s_end++;
    s_start = s_end;
  }
  n = s_end;

  r -> zones_list = 
    malloc(r -> number_of_zones * sizeof(struct zone));
  z_start = n;
  z_end = n + 1;
  temp = "end_zone";
  for(i = 0; i < r -> number_of_zones; i++){
    r -> zones_list[i] = createZone((p + s_start));
    now = numberOfWords(p[z_end]);
    params = divideLine(p[z_end], now, MAX_STRING_LENGTH);
    assert(params != 0);
    while(!compTwoStrings(temp, p[s_end])){
      freeCharArray(params, now);
      s_end++;
      now = numberOfWords(p[z_end]);
      params = divideLine(p[z_end], now, MAX_STRING_LENGTH);
      assert(params != 0);
    }
    freeCharArray(params, now);
    s_end++;
    s_start = s_end;
  }
  n = s_end;

  return r;
}

/*
 *createSpeed_limit function
 *
 *line : a pointer to a string represinting a speed_limit
 *
 *this function will take a string represinting a speed_limit
 *and return a struct speed_limit
 */
struct speed_limit *createSpeed_limit(char *line)
{
  int now;
  char **params;
  struct speed_limit *sl;
  
  sl = malloc(sizeof(struct speed_limit));
  assert(sl != 0);
  now = numberOfWords(line);
  assert(now == 3);
  params = divideLine(line, now, MAX_STRING_LENGTH);
  assert(params != 0);
  sl -> id = atoi(params[0]);
  sl -> min_speed = atoi(params[1]);
  sl -> max_speed = atoi(params[2]);
  freeCharArray(params, now);

  return sl;
}

/*
 *createMDF function
 *
 *p : a pointer to an array of strings represinting a MDF file
 *
 *this function will take an array of strings represinting a MDF
 *and return a struct RNDF
 */
struct MDF *createMDF(char **p)
{
  struct MDF *m;
  char *temp;
  char **params;
  int now;
  int n = 0;
  int i;

  m = malloc(sizeof(struct MDF));
  assert(m != 0);
  now = numberOfWords(p[n]);
  assert(now == 2);
  params = divideLine(p[n], now, MAX_STRING_LENGTH);
  assert(params != 0);
  m -> MDF_name = malloc(MAX_STRING_LENGTH * sizeof(char));
  stringCopy(params[1], m -> MDF_name);
  freeCharArray(params, now);
  n++;

  now = numberOfWords(p[n]);
  assert(now == 2);
  params = divideLine(p[n], now, MAX_STRING_LENGTH);
  assert(params != 0);
  m -> RNDF_name = malloc(MAX_STRING_LENGTH * sizeof(char));
  stringCopy(params[1], m -> RNDF_name);
  freeCharArray(params, now);
  n++;

  temp = "format_version";
  now = numberOfWords(p[n]);
  params = divideLine(p[n], now, MAX_STRING_LENGTH);
  assert(params != 0);
  if(compTwoStrings(params[0], temp)){
    assert(now == 2);
    m -> format_version = malloc(MAX_STRING_LENGTH * sizeof(char));
    stringCopy(params[1], m -> format_version);
    n++;
  }
  else
    m -> format_version = 0;
  freeCharArray(params, now);

  temp = "creation_date";
  now = numberOfWords(p[n]);
  params = divideLine(p[n], now, MAX_STRING_LENGTH);
  assert(params != 0);
  if(compTwoStrings(params[0], temp)){
    assert(now == 2);
    m -> creation_date = malloc(MAX_STRING_LENGTH * sizeof(char));
    stringCopy(params[1], m -> creation_date);
    n++;
  }
  else
    m -> creation_date = 0;
  freeCharArray(params, now);

  temp = "checkpoints";
  now = numberOfWords(p[n]);
  assert(now == 1);
  params = divideLine(p[n], now, MAX_STRING_LENGTH);
  assert(params != 0);
  assert(compTwoStrings(params[0], temp));
  freeCharArray(params, now);
  n++;

  now = numberOfWords(p[n]);
  assert(now == 2);
  params = divideLine(p[n], now, MAX_STRING_LENGTH);
  assert(params != 0);
  m -> number_of_checkpoints = atoi(params[1]);
  freeCharArray(params, now);
  n++;

  m -> checkpoints_list = malloc((m -> number_of_checkpoints) * sizeof(int));
  assert((m -> checkpoints_list) != 0);
  for(i = 0; i < m -> number_of_checkpoints; i++){
    now = numberOfWords(p[n]);
    assert(now == 1);
    params = divideLine(p[n], now, MAX_STRING_LENGTH);
    assert(params != 0);
    m -> checkpoints_list[i] = atoi(params[0]);
    freeCharArray(params, now);
    n++;
  }

  temp = "end_checkpoints";
  now = numberOfWords(p[n]);
  assert(now == 1);
  params = divideLine(p[n], now, MAX_STRING_LENGTH);
  assert(params != 0);
  assert(compTwoStrings(params[0], temp));
  freeCharArray(params, now);
  n++;

  temp = "speed_limits";
  now = numberOfWords(p[n]);
  assert(now == 1);
  params = divideLine(p[n], now, MAX_STRING_LENGTH);
  assert(params != 0);
  assert(compTwoStrings(params[0], temp));
  freeCharArray(params, now);
  n++;

  now = numberOfWords(p[n]);
  assert(now == 2);
  params = divideLine(p[n], now, MAX_STRING_LENGTH);
  assert(params != 0);
  m -> number_of_speed_limits = atoi(params[1]);
  freeCharArray(params, now);
  n++;

  m -> speed_limits_list = malloc((m -> number_of_speed_limits) * 
			       sizeof(struct speed_limit));
  assert((m -> speed_limits_list) != 0);
  for(i = 0; i < m -> number_of_speed_limits; i++){
    m -> speed_limits_list[i] = createSpeed_limit(p[n]);
    n++;
  }

  temp = "end_speed_limits";
  now = numberOfWords(p[n]);
  assert(now == 1);
  params = divideLine(p[n], now, MAX_STRING_LENGTH);
  assert(params != 0);
  assert(compTwoStrings(params[0], temp));
  freeCharArray(params, now);
  n++;

  return m;
}

/*
 *freeID function
 *
 *i : a pointer to struct id
 *
 *this function will free the space allocated for a struct id
 */
void freeId(struct id *i)
{
  free(i);
}

/*
 *freeWaypoint function
 *
 *w : a pointer to struct waypoint
 *
 *this function will free the space allocated for a struct waypoint
 */
void freeWaypoint(struct waypoint *w)
{
  freeId(w -> waypoint_id);
  free(w);
}

/*
 *freeCheckpoint function
 *
 *c : a pointer to struct checkpoint
 *
 *this function will free the space allocated for a struct checkpoint
 */
void freeCheckpoint(struct checkpoint *c)
{
  freeId(c -> waypoint_id);
  free(c);
}

/*
 *freeStop function
 *
 *s : a pointer to struct stop
 *
 *this function will free the space allocated for a struct stop
 */
void freeStop(struct stop *s)
{
  freeId(s -> waypoint_id);
  free(s);
}

/*
 *freeExit function
 *
 *e : a pointer to struct exit
 *
 *this function will free the space allocated for a struct exit
 */
void freeExit(struct exit *e)
{
  freeId(e -> exit_waypoint_id);
  freeId(e -> entry_point_id);
  free(e);
}

/*
 *freeLane function
 *
 *l : a pointer to struct lane
 *
 *this function will free the space allocated for a struct lane
 */
void freeLane(struct lane *l)
{
  int i;
  
  freeId(l -> lane_id);
  if(l -> left_boundary != 0)
    free(l -> left_boundary);
  if(l -> right_boundary != 0)
    free(l -> right_boundary);
  for(i = 0; i < l -> number_of_checkpoints; i++)
    freeCheckpoint(l -> checkpoints_list[i]);
  if(l -> checkpoints_list != 0)
    free(l -> checkpoints_list);
  for(i = 0; i < l -> number_of_stops; i++)
    freeStop(l -> stops_list[i]);
  if(l -> stops_list != 0)
    free(l -> stops_list);
  for(i = 0; i < l -> number_of_exits; i++)
    freeExit(l -> exits_list[i]);
  if(l -> exits_list != 0)
    free(l -> exits_list);
  for(i = 0; i < l -> number_of_waypoints; i++)
    freeWaypoint(l -> waypoints_list[i]);
  free(l -> waypoints_list);
  free(l);
}

/*
 *freeSegment function
 *
 *s : a pointer to struct segment
 *
 *this function will free the space allocated for a struct segment
 */
void freeSegment(struct segment *s)
{
  int i;
  
  if(s -> segment_name != 0)
    free(s -> segment_name);
  for(i = 0; i < s -> number_of_lanes; i++)
    freeLane(s -> lanes_list[i]);
  free(s -> lanes_list);
  free(s);
}

/*
 *freePerimeterpoint function
 *
 *p : a pointer to struct perimeterpoint
 *
 *this function will free the space allocated for a struct perimeterpoint
 */
void freePerimeterpoint(struct perimeterpoint *p)
{
  freeId(p -> perimeterpoint_id);
  free(p);
}

/*
 *freePerimeter function
 *
 *p : a pointer to struct perimeter
 *
 *this function will free the space allocated for a struct perimeter
 */
void freePerimeter(struct perimeter *p)
{
  int i;
  
  freeId(p -> perimeter_id);
  for(i = 0; i < p -> number_of_exits; i++)
    freeExit(p -> exits_list[i]);
  if(p -> exits_list)
    free(p -> exits_list);
  for(i = 0; i < p -> number_of_perimeterpoints; i++)
    freePerimeterpoint(p -> perimeterpoints_list[i]);
  free(p -> perimeterpoints_list);
  free(p);
}

/*
 *freeSpot function
 *
 *s : a pointer to struct spot
 *
 *this function will free the space allocated for a struct spot
 */
void freeSpot(struct spot *s)
{
  freeId(s -> spot_id);
  if(s -> check_waypoint != 0)
    freeCheckpoint(s -> check_waypoint);
  freeWaypoint(s -> waypoint1);
  freeWaypoint(s -> waypoint2);
  free(s);
}

/*
 *freeZone function
 *
 *z : a pointer to struct zone
 *
 *this function will free the space allocated for a struct zone
 */
void freeZone(struct zone *z)
{
  int i;
  
  if(z -> zone_name != 0)
    free(z -> zone_name);
  freePerimeter(z -> zone_perimeter);
  for(i = 0; i < z -> number_of_parking_spots; i++)
    freeSpot(z -> parking_spots_list[i]);
  free(z -> parking_spots_list);
  free(z);
}

/*
 *freeRNDF function
 *
 *r : a pointer to struct RNDF
 *
 *this function will free the space allocated for a struct RNDF
 */
void freeRNDF(struct RNDF *r)
{
  int i;

  free(r -> RNDF_name);
  if(r -> format_version != 0)
    free(r -> format_version);
  if(r -> creation_date != 0)
    free(r -> creation_date);
  for(i = 0; i < r -> number_of_segments; i++)
    freeSegment(r -> segments_list[i]);
  free(r -> segments_list);
  for(i = 0; i < r -> number_of_zones; i++)
    freeZone(r -> zones_list[i]);
  free(r -> zones_list);
  free(r);
}

/*
 *freeSpeed_limit function
 *
 *sl : a pointer to struct speed_limit
 *
 *this function will free the space allocated for a struct speed_limit
 */
void freeSpeed_limit(struct speed_limit *sl)
{
  free(sl);
}

/*
 *freeMDF function
 *
 *m : a pointer to struct MDF
 *
 *this function will free the space allocated for a struct MDF
 */
void freeMDF(struct MDF *m)
{
  int i;

  free(m -> MDF_name);
  free(m -> RNDF_name);
  if(m -> format_version != 0)
    free(m -> format_version);
  if(m -> creation_date != 0)
    free(m -> creation_date);
  free(m -> checkpoints_list);
  for(i = 0; i < m -> number_of_speed_limits; i++)
    freeSpeed_limit(m -> speed_limits_list[i]);
  free(m -> speed_limits_list);
  free(m);
}

/*
 *parseAnalyzeRndfFile function
 *
 *file : a pointer to file RNDF file descriptor
 *
 *this function will take a RNDF file descriptor and return
 *a pointer to a RNDF struct represinting the information in the file
 */
struct RNDF *parseAnalyzeRndfFile(FILE *file)
{
  int max_nol;
  char **arr;
  char **arr2;
  char **arr3;
  struct RNDF *res;

  max_nol = numberOfLines(file);
  rewind(file);
  arr = getArrayOfLines(file, max_nol, 1000);
  fclose(file);

  arr2 = removeComments(arr, max_nol, 1000);
  freeCharArray(arr, max_nol);
  arr3 = removeEmptyLines(arr2, max_nol, 1000);
  freeCharArray(arr2, max_nol);

  res = createRNDF(arr3);
  freeCharArray(arr3, max_nol);

  return res;
}

/*
 *parseAnalyzeMdfFile function
 *
 *file : a pointer to file MDF file descriptor
 *
 *this function will take a MDF file descriptor and return
 *a pointer to a MDF struct represinting the information in the file
 */
struct MDF *parseAnalyzeMdfFile(FILE *file)
{
  int max_nol;
  char **arr;
  char **arr2;
  char **arr3;
  struct MDF *res;

  max_nol = numberOfLines(file);
  rewind(file);
  arr = getArrayOfLines(file, max_nol, 1000);
  fclose(file);

  arr2 = removeComments(arr, max_nol, 1000);
  freeCharArray(arr, max_nol);
  arr3 = removeEmptyLines(arr2, max_nol, 1000);
  freeCharArray(arr2, max_nol);

  res = createMDF(arr3);
  freeCharArray(arr3, max_nol);

  return res;
}

