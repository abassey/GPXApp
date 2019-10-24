#include "GPXParser.h"
#include "helper.h"
#include "LinkedListAPI.h"
#include <stdio.h>

GPXdoc* createGPXdoc(char* fileName)
{
    if(fileName == NULL) return NULL;

    xmlDoc * doc = NULL;
    xmlNode * root_element = NULL;

    LIBXML_TEST_VERSION

    doc = xmlReadFile(fileName, NULL, 0);
    root_element = xmlDocGetRootElement(doc);

    if (doc == NULL) return NULL;
    
    //printf("in gdoc creator\n");

    GPXdoc * gdoc = new_gpxdoc();
    
    create_doc_from_tree(root_element, &gdoc);
    
    xmlFreeDoc(doc);
    xmlCleanupParser();
    return gdoc;
}

char* GPXdocToString(GPXdoc* doc)
{
    if(doc == NULL) return NULL;

	char * temp = new_string();
	char buffer[50];
	
	strcpy(temp, "namespace: ");
	strcat(temp, doc->namespace);
	strcat(temp, ", version: ");
	snprintf(buffer, sizeof(buffer), "%.2f", doc->version);
	strcat(temp, buffer);
	strcat(temp, ", creator: ");
	strcat(temp, doc->creator);
	strcat(temp, ", waypoints: [");
	strcat(temp, toString(doc->waypoints));
	strcat(temp, "], routes: [");
	strcat(temp, toString(doc->routes));
	strcat(temp, "], tracks: [");
	strcat(temp, toString(doc->tracks));
	strcat(temp, "]");
	
	return temp;
	
}

void deleteGPXdoc(GPXdoc* doc)
{
	if(doc == NULL) return;
	
    free_string(doc->creator);

    //free each waypoint with list iterator then free whole list
    freeList(doc->waypoints);

    //free each route with list iterator then free whole list
    freeList(doc->routes);

    //free each track with list iterator then free whole list
    freeList(doc->tracks);
    free(doc);
}

int getNumWaypoints(const GPXdoc* doc)
{
	if(doc == NULL) return -1;
	
    int num_waypoints = 0;

    num_waypoints = getLength(doc->waypoints);

    return num_waypoints;
}

int getNumRoutes(const GPXdoc* doc)
{
	if(doc == NULL) return -1;
	
    int num_routes = 0;

    num_routes = getLength(doc->routes);

    return num_routes;
}

int getNumTracks(const GPXdoc* doc)
{
	if(doc == NULL) return -1;
	
    int num_tracks = 0;

    num_tracks = getLength(doc->tracks);

    return num_tracks;
}

int getNumSegments(const GPXdoc* doc)
{
	if(doc == NULL) return -1; 
	
    int num_segments = 0;

    //num_segments = getLength(doc->waypoints->segments);
    
    ListIterator itr = createIterator(doc->tracks);

	Track * data = nextElement(&itr);
	while (data != NULL)
	{
		num_segments += getLength(data->segments);
		
		data = nextElement(&itr);
	}

    return num_segments;
}

int getNumAttributes(const GPXdoc* doc)
{
	if(doc == NULL) return -1; 
	
	int num_attributes = 0;
	
	ListIterator itr = createIterator(doc->waypoints);
	
	Waypoint * wp = nextElement(&itr);
	while (wp != NULL)
	{
		if(strlen(wp->name) != 0) num_attributes++;
		
		num_attributes += getLength(wp->attributes);
		
		wp = nextElement(&itr);
	}
	
	itr = createIterator(doc->routes);
	
	//getting num for routes and its waypoints
	Route * rte = nextElement(&itr);
	while (rte != NULL)
	{
		ListIterator inner_itr = createIterator(rte->waypoints);
				
		Waypoint * rte_wp = nextElement(&inner_itr);
		
		while (rte_wp != NULL)
		{
			if(strlen(rte_wp->name) != 0) num_attributes++;
			
			num_attributes += getLength(rte_wp->attributes);
			
			rte_wp = nextElement(&inner_itr);
		}
		
		if(strlen(rte->name) != 0) num_attributes++;
		
		num_attributes += getLength(rte->attributes);
		
		rte = nextElement(&itr);
	}	
	
	itr = createIterator(doc->tracks);

	//getting num for tracks and track segments
	Track * trk = nextElement(&itr);
	while (trk != NULL)
	{
		ListIterator seg_itr = createIterator(trk->segments);
		
		TrackSegment * ts = nextElement(&seg_itr);
			while (ts != NULL)
			{
				ListIterator inner_itr = createIterator(ts->waypoints);
				
				Waypoint * ts_wp = nextElement(&itr);
				
				while (ts_wp != NULL)
				{
					if(strlen(ts_wp->name) != 0) num_attributes++;
					
					num_attributes += getLength(ts_wp->attributes);
					
					ts_wp = nextElement(&inner_itr);
				}
				
				ts = nextElement(&seg_itr);
			}
		
		if(strlen(trk->name) != 0) num_attributes++;
			
		num_attributes += getLength(trk->attributes);
		
		trk = nextElement(&itr);
	}
	
	return num_attributes;
}

Waypoint* getWaypoint(const GPXdoc* doc, char* name)
{
	if(doc == NULL || name == NULL) return NULL;
	
    Waypoint * found_waypoint = new_waypoint();
	strcpy(found_waypoint->name, name);
	
	ListIterator itr = createIterator(doc->waypoints);
	
	Waypoint * data = nextElement(&itr);
	while (data != NULL)
	{
		if (compareWaypoints(data, found_waypoint))
			{
				found_waypoint->longitude = data->longitude;
				found_waypoint->latitude = data->latitude;
				found_waypoint->attributes = data->attributes;
				return found_waypoint;
			}

		data = nextElement(&itr);
	}
	
	itr = createIterator(doc->routes);
	
	//getting num for routes and its waypoints
	Route * rte = nextElement(&itr);
	while (rte != NULL)
	{
		ListIterator inner_itr = createIterator(rte->waypoints);
				
		data = nextElement(&inner_itr);
			while (data != NULL)
			{
				if (compareWaypoints(data, found_waypoint))
					{
						found_waypoint->longitude = data->longitude;
						found_waypoint->latitude = data->latitude;
						found_waypoint->attributes = data->attributes;
						return found_waypoint;
					}

				data = nextElement(&inner_itr);
			}
		
		rte = nextElement(&itr);
	}	
	
	itr = createIterator(doc->tracks);

	//getting num for tracks and track segments
	Track * trk = nextElement(&itr);
	while (trk != NULL)
	{
		ListIterator seg_itr = createIterator(trk->segments);
		
		TrackSegment * ts = nextElement(&seg_itr);
			while (ts != NULL)
			{
				ListIterator inner_itr = createIterator(ts->waypoints);
				
				data = nextElement(&inner_itr);
					while (data != NULL)
					{
						if (compareWaypoints(data, found_waypoint) == 0)
							{
								found_waypoint->longitude = data->longitude;
								found_waypoint->latitude = data->latitude;
								found_waypoint->attributes = data->attributes;
								return found_waypoint;
							}

						data = nextElement(&inner_itr);
					}
				
				ts = nextElement(&seg_itr);
			}
		trk = nextElement(&itr);
	}

	return NULL;
}

Track* getTrack(const GPXdoc* doc, char* name)
{
	if(doc == NULL || name == NULL) return NULL;
	
    Track * found_track = new_track();
    strcpy(found_track->name, name);

    ListIterator itr = createIterator(doc->tracks);

	Track * data = nextElement(&itr);
	while (data != NULL)
	{
		if (compareTracks(data, found_track) == 0)
			{
				found_track->segments = data->segments;
				found_track->attributes = data->attributes;
				return found_track;
			}

		data = nextElement(&itr);
	}

	return NULL;
}

Route* getRoute(const GPXdoc* doc, char* name)
{
	 if(doc == NULL) return NULL;
	 
     Route * found_route = new_route();
     strcpy(found_route->name, name);
     
     ListIterator itr = createIterator(doc->routes);

	Route * data = nextElement(&itr);
	while (data != NULL)
	{
		if (compareRoutes(data, found_route) == 0)
			{
				found_route->waypoints = data->waypoints;
				found_route->attributes = data->attributes;
				return found_route;
			}

		data = nextElement(&itr);
	}

	return NULL;
}


/* ******************************* List helper functions  - MUST be implemented *************************** */

void deleteAttribute( void* data)
{
    Attribute * temp = (Attribute *) data;
    free(temp->name);
    free(temp->value);
    free(temp);
}

char* attributeToString( void* data)
{
	Attribute * att = (Attribute *) data;
    char * temp = new_string();
    strcpy(temp, "name: ");
    strcat(temp, att->name);
    strcat(temp, ", ");
    strcat(temp, "value: ");
    strcat(temp, att->value);
    
    return temp;

}

int compareAttributes(const void *first, const void *second)
{
	const Attribute * a_first = (const Attribute *) first;
	const Attribute * b_second = (const Attribute *) second;
	
	return (strcmp(a_first->name, b_second->name) && strcmp(a_first->value, b_second->value));
}

void deleteWaypoint(void* data)
{
    Waypoint * temp = (Waypoint *) data;
    free(temp->name);
    freeList(temp->attributes);
    free(temp);
}

char* waypointToString( void* data)
{
    char buffer[50];

    Waypoint * wp = (Waypoint *) data;
    char * temp = new_string();
    strcpy(temp,"name: ");
    strcat(temp,wp->name);
    strcat(temp,", ");

    strcpy(temp,"longitude: ");
    snprintf(buffer, sizeof(buffer), "%f", wp->longitude);
    strcat(temp, buffer);
    strcat(temp,", ");

    strcat(temp,"latitude: ");
    snprintf(buffer, sizeof(buffer), "%f", wp->latitude);
    strcat(temp,buffer);
    strcat(temp,", [");

    strcat(temp,toString(wp->attributes));
    strcat(temp, "]");
    return temp;
}

int compareWaypoints(const void *first, const void *second)
{
	const Waypoint * a_first = (const Waypoint *) first;
	const Waypoint * b_second = (const Waypoint *) second;
	
	return strcmp(a_first->name, b_second->name);
}

void deleteRoute(void* data)
{
    Route * temp = (Route *) data;
    free(temp->name);
    freeList(temp->waypoints);
    freeList(temp->attributes);
    free(temp);
}

char* routeToString(void* data)
{
	Route * rt = (Route *) data;
	
	char * temp = new_string();
	strcpy(temp, "name: ");
	strcat(temp, rt->name);
	strcat(temp, ", [");
	strcat(temp, toString(rt->waypoints));
	strcat(temp, "], [");
	strcat(temp, toString(rt->attributes));
	strcat(temp, " ]");
	
	return temp;
}

int compareRoutes(const void *first, const void *second)
{
	const Route * a_first = (const Route *) first;
	const Route * b_second = (const Route *) second;
	
	return (strcmp(a_first->name, b_second->name));
}

void deleteTrackSegment(void* data)
{
	TrackSegment * temp = (TrackSegment *) data;
	freeList(temp->waypoints);
	free(temp);
}

char* trackSegmentToString(void* data)
{
	TrackSegment * ts = (TrackSegment *) data;
	char * temp = new_string();
	
	strcpy(temp, toString(ts->waypoints));
	
	return temp;
}

int compareTrackSegments(const void *first, const void *second)
{
	if(first == second) return 0;
	else if (first < second) return -1;
	 
	return 1;
}

void deleteTrack(void* data)
{	
	Track * temp = (Track *) data;
	free(temp->name);
	freeList(temp->segments);
	freeList(temp->attributes);
	free(temp);
}

char* trackToString(void* data)
{
	Track * tr = (Track *) data;
	char * temp = new_string();
	
	strcpy(temp, "name: ");
	strcat(temp, tr->name);
	strcat(temp, ", [");
	strcat(temp, toString(tr->segments));
	strcat(temp, "] , [");
	strcat(temp, toString(tr->attributes));
	strcat(temp, "] ");
	
	return temp;
}

int compareTracks(const void *first, const void *second)
{
	const Track * a_first = (const Track *) first;
	const Track * b_second = (const Track *) second;
	
	return (strcmp(a_first->name, b_second->name));
}


bool validateGPXDoc(GPXdoc* doc, char* gpxSchemaFile)
{
	bool valid = false;
	
	if(gpxSchemaFile == NULL) return NULL;
	if(doc == NULL) return NULL;
	
	xmlDocPtr xml_doc = NULL;
	xmlNodePtr root = NULL;
	
	LIBXML_TEST_VERSION;
	
	xml_doc = xmlNewDoc(BAD_CAST "1.0");
	root = xmlNewNode(NULL, BAD_CAST "gpx");
	xmlDocSetRootElement(xml_doc, root); 
	
	
	create_tree_from_doc( &root, doc);
	
	//xmlSaveFormatFileEnc("gpx.xml" , xml_doc, "UTF-8", 1);
	
	xmlSchemaPtr schema = NULL;
	xmlSchemaParserCtxtPtr ctxt;
	//char *XMLFileName = "gpx.xml";


	xmlLineNumbersDefault(1);

	ctxt = xmlSchemaNewParserCtxt(gpxSchemaFile);

	xmlSchemaSetParserErrors(ctxt, (xmlSchemaValidityErrorFunc) fprintf, (xmlSchemaValidityWarningFunc) fprintf, stderr);
	schema = xmlSchemaParse(ctxt);
	xmlSchemaFreeParserCtxt(ctxt);
	//xmlSchemaDump(stdout, schema); //To print schema dump

	//xml_doc = xmlReadFile(XMLFileName, NULL, 0);

	if (xml_doc == NULL)
	{ 
		fprintf(stderr, "Could not parse");
	} else {
		xmlSchemaValidCtxtPtr ctxt;
		int ret;

		ctxt = xmlSchemaNewValidCtxt(schema);
		xmlSchemaSetValidErrors(ctxt, (xmlSchemaValidityErrorFunc) fprintf, (xmlSchemaValidityWarningFunc) fprintf, stderr);
		ret = xmlSchemaValidateDoc(ctxt, xml_doc);
		if (ret == 0)
		{
			//printf("%s validates\n", XMLFileName);
			valid = true;
		}else if (ret > 0) {
			//printf("%s fails to validate\n", XMLFileName);
			valid = false;
		} else {
			//printf("%s validation generated an internal error\n", XMLFileName);
			return NULL;
		}
		xmlSchemaFreeValidCtxt(ctxt);
		xmlFreeDoc(xml_doc);
	}

	// free the resource
	if(schema != NULL)
	xmlSchemaFree(schema);

	xmlSchemaCleanupTypes();
	//sxmlFreeDoc(xml_doc);
	xmlCleanupParser();
	xmlMemoryDump();
	
	return valid;
}

bool writeGPXdoc(GPXdoc* doc, char* fileName)
{
	bool write = false;
	
	if(fileName == NULL) return NULL;
	if(doc == NULL) return NULL;
	
	write = validateGPXDoc(doc, "gpx.xsd");
	
	if(write == true)
	{
		xmlDocPtr xml_doc = NULL;
		xmlNodePtr root = NULL;
		
		LIBXML_TEST_VERSION;
		
		xml_doc = xmlNewDoc(BAD_CAST "1.0");
		root = xmlNewNode(NULL, BAD_CAST "gpx");
		xmlDocSetRootElement(xml_doc, root); 
		
		create_tree_from_doc( &root, doc);
		
		xmlSaveFormatFileEnc(fileName , xml_doc, "UTF-8", 1);
	} else {
		fprintf(stderr, "Error writing to file\n\n");
	}
	
	return write;
}

float getRouteLen(const Route *rt)
{
	if(rt == NULL) return 0;
	
	float len = 0.0;
	
	int arr_len = getLength(rt->waypoints);
	float arr[arr_len][2];
	int i = 0;
	
	ListIterator itr = createIterator(rt->waypoints);
	Waypoint * rtept = nextElement(&itr);
	
	while(rtept != NULL)
	{
		arr[i][0] = rtept->latitude;
		arr[i][1] = rtept->longitude;
		//printf("Lat: %lf,Lon: %lf\n", arr[i][0], arr[i][1]);
		
		i++;
		
		rtept = nextElement(&itr);
	}
	
	i = 0;
	
	for(i = 0; i < arr_len - 1; i++)
	{
		len += dist_calc(arr[i][0], arr[i][1], arr[i + 1][0], arr[i + 1][1]); 
	}
	
	return len * 1000;
}

//find out why not working for round10
float round10(float len)
{
	if(len < 0) return -1;
	
	int n = len;
    int a = (n / 10) * 10; 
    int b = a + 10; 
  
    return (float)(n - a >= b - n)? b : a; 

}


float getTrackLen(const Track *tr)
{
	if(tr == NULL) return 0;
	
	float len = 0.0;
	
	if(getLength(tr->segments) == 1) {
			
		TrackSegment * trkseg = getFromFront(tr->segments);
	
		int arr_len = getLength(trkseg->waypoints);
		float arr[arr_len][2];
		int i = 0;
		
		ListIterator itr = createIterator(trkseg->waypoints);
		Waypoint * trkpt = nextElement(&itr);
		
		while(trkpt != NULL)
		{
			arr[i][0] = trkpt->latitude;
			arr[i][1] = trkpt->longitude;
			//printf("Lat: %lf,Lon: %lf\n", arr[i][0], arr[i][1]);
			
			i++;
			
			trkpt = nextElement(&itr);
		}
		
		i = 0;
		
		for(i = 0; i < arr_len - 1; i++)
		{
			len += dist_calc(arr[i][0], arr[i][1], arr[i + 1][0], arr[i + 1][1]); 
			//printf("curr len = %f\n", len);
		}
	} else {
		//comeback to this part
		
		//thinking to do a loop that goes through each track seg, computes the length, then
		//stores the length of that track seg in a length array, then add up the elements in
		//the array. call insertFront and insertBack on the last and first points to the calculate, and
		//add to total length
		
		ListIterator itr = createIterator(tr->segments);
		TrackSegment * trkseg = nextElement(&itr);
		
		int endpoints_len = getLength(tr->segments);
		int endpoints[endpoints_len * 2][2];
		int j = 0;
		
		while(trkseg != NULL)
		{
			Waypoint * back = getFromBack(trkseg->waypoints);
			endpoints[j][0] = back->latitude;
			endpoints[j][1] = back->longitude;
			j++;
			
			int arr_len = getLength(trkseg->waypoints);
			float arr[arr_len][2];
			int i = 0;
			
			ListIterator inner_itr = createIterator(trkseg->waypoints);
			Waypoint * trkpt = nextElement(&inner_itr);
			
			while(trkpt != NULL)
			{
				arr[i][0] = trkpt->latitude;
				arr[i][1] = trkpt->longitude;
			//printf("Lat: %lf,Lon: %lf\n", arr[i][0], arr[i][1]);
			
				i++;
			
				trkpt = nextElement(&inner_itr);
			}
			
			i = 0;
		
			for(i = 0; i < arr_len - 1; i++)
			{
				len += dist_calc(arr[i][0], arr[i][1], arr[i + 1][0], arr[i + 1][1]); 
				//printf("curr len = %f\n", len);
			}
			
			if((trkseg = nextElement(&itr)) != NULL)
			{
				Waypoint * front;
				front = getFromFront(trkseg->waypoints);
				endpoints[j][0] = front->latitude;
				endpoints[j][1] = front->longitude;
				j++;
			}
		}
			
			int k = j;
			j = 0;
		
			for(j = 0; j < k - 1; j++)
			{
				len += dist_calc(endpoints[j][0], endpoints[j][1], endpoints[j + 1][0], endpoints[j + 1][1]); 
				//printf("curr len = %f\n", len);
			}
		
	}
	
	return len * 1000;
}


int numRoutesWithLength(const GPXdoc* doc, float len, float delta)
{
	if(doc == NULL) return 0;
	if(len < 0) return 0;
	if(delta < 0) return 0;
	
	int num = 0;
	
	float rte_len = 0.0;
	
	ListIterator itr = createIterator(doc->routes);
	Route * rte = nextElement(&itr);
	
	while(rte != NULL)
	{
		rte_len = getRouteLen((const Route *)rte);
		
		if(abs(len - rte_len) <=  delta) num++;
		
		rte = nextElement(&itr);
	}
	
	return num;
}


int numTracksWithLength(const GPXdoc* doc, float len, float delta)
{
	if(doc == NULL) return 0;
	if(len < 0) return 0;
	if(delta < 0) return 0;
	
	int num = 0;
	
	float trk_len = 0.0;
	
	ListIterator itr = createIterator(doc->tracks);
	Track * trk = nextElement(&itr);
	
	while(trk != NULL)
	{
		trk_len = getTrackLen((const Track *)trk);
		
		if(abs(len - trk_len) <=  delta) num++;
		
		trk = nextElement(&itr);
	}
	
	return num;
}

bool isLoopRoute(const Route* route, float delta)
{
	bool loop = false;
	
	if(route == NULL) return false;
	if(delta < 0)return false;
	
	int len = getLength(route->waypoints);
	float rte_len = 0.0;
	
	if(len < 4) return false;
	else {
		Waypoint * front = getFromFront(route->waypoints);
		Waypoint * back = getFromBack(route->waypoints);
		
		rte_len = dist_calc(front->latitude, front->longitude, back->latitude, back->longitude);
		if((rte_len * 1000) <= delta) loop = true;
	}
	
	return loop;
}

bool isLoopTrack(const Track *tr, float delta)
{
	bool loop = false;
	
	if(tr == NULL) return false;
	if(delta < 0)return false;
	
	int len = getLength(tr->segments);
	float trk_len = 0.0;
	
	if(len < 1) return false;
	else {
		
		ListIterator itr = createIterator(tr->segments);
		TrackSegment * trkseg = nextElement(&itr);
		
		while(trkseg != NULL)
		{
			Waypoint * front = getFromFront(trkseg->waypoints);
			Waypoint * back = getFromBack(trkseg->waypoints);
			
			trk_len = dist_calc(front->latitude, front->longitude, back->latitude, back->longitude);
			if((trk_len * 1000) <= delta) 
			{
				loop = true;
				return loop;
			}
			
			trkseg = nextElement(&itr);
		}
	}
	
	return loop;
}


List* getRoutesBetween(const GPXdoc* doc, float sourceLat, float sourceLong, float destLat, float destLong, float delta)
{
	if(doc == NULL) return NULL;
	
	char* (*rteToStr)(void*) = &routeToString;
	void (*delRte)(void*) = &deleteRoute;
	int (*compRte)(const void*, const void*) = &compareRoutes;
	
	List * shallowRtes = initializeList(rteToStr, delRte, compRte);
	
	float end_dest = 0.0;
	float source_dest = 0.0;
	
	ListIterator itr = createIterator(doc->routes);
	Route * rte = nextElement(&itr);
	
	while(rte != NULL)
	{
		Waypoint * front = getFromFront(rte->waypoints);
		Waypoint * back = getFromBack(rte->waypoints);
		
		source_dest = dist_calc(sourceLat, sourceLong, front->latitude, front->longitude);
		end_dest = dist_calc(destLat, destLong, back->latitude, back->longitude);
		
		if(abs(source_dest - end_dest) <= delta) insertBack(shallowRtes, rte);
		else shallowRtes->length = 0;
		
		rte = nextElement(&itr);
	}
	
	if(getLength(shallowRtes) == 0) return NULL;
	return shallowRtes;
}

List* getTracksBetween(const GPXdoc* doc, float sourceLat, float sourceLong, float destLat, float destLong, float delta)
{
	if(doc == NULL) return NULL;
	
	char* (*trToStr)(void*) = &trackToString;
	void (*delTr)(void*) = &deleteTrack;
	int (*compTr)(const void*, const void*) = &compareTracks;
	
	List * shallowTrks = initializeList(trToStr, delTr, compTr);
	
	float end_dest = 0.0;
	float source_dest = 0.0;
	
	ListIterator itr = createIterator(doc->tracks);
	Track * trk = nextElement(&itr);
	
	while(trk != NULL)
	{
		ListIterator inner_itr = createIterator(trk->segments);
		TrackSegment * trkseg = nextElement(&inner_itr);
		
		while(trkseg != NULL)
		{
			Waypoint * front = getFromFront(trkseg->waypoints);
			Waypoint * back = getFromBack(trkseg->waypoints);
			
			source_dest = dist_calc(sourceLat, sourceLong, front->latitude, front->longitude);
			end_dest = dist_calc(destLat, destLong, back->latitude, back->longitude);
			
			if(abs(source_dest - end_dest) <= delta) insertBack(trk->segments, trkseg);
			
			trkseg = nextElement(&inner_itr);
		}
		
		if(getLength(trk->segments) != 0) insertBack(shallowTrks, trk);
		
		trk = nextElement(&itr);
	}
	
	if(getLength(shallowTrks) == 0) return NULL;
	return shallowTrks;
}


char* trackToJSON(const Track *tr)
{
	if(tr == NULL) return "{}";
	
	char * string = new_string();
	float len = getTrackLen(tr);
	len = round10(len);
	char buffer[50];
	bool loop = isLoopTrack(tr, 15);
	
	strcpy(string, "{\"name\":\"");
	if(strcmp(tr->name, "\0") == 0) strcat(string, "None");
	else strcat(string, tr->name);
	
	strcat(string, "\",\"len\":");
	snprintf(buffer, sizeof(buffer), "%.1f", len);
	strcat(string, buffer);
	
	strcat(string, ",\"loop\":");
	if(loop) strcat(string, "true}");
	else strcat(string, "false}");
	
	return string;
}

char* routeToJSON(const Route *rt)
{
	if(rt == NULL) return "{}";
	
	char * string = new_string();
	int num = getLength(rt->waypoints);
	float len = getRouteLen(rt);
	len = round10(len);
	char buffer[50];
	bool loop = isLoopRoute(rt, 15);
	
	strcpy(string, "{\"name\":\"");
	if(strcmp(rt->name, "\0") == 0) strcat(string, "None");
	else strcat(string, rt->name);
	
	strcat(string, "\",\"numPoints\":");
	snprintf(buffer, sizeof(buffer), "%d", num);
	strcat(string, buffer);
	
	strcat(string, ",\"len\":");
	snprintf(buffer, sizeof(buffer), "%.1f", len);
	strcat(string, buffer);
	
	strcat(string, ",\"loop\":");
	if(loop) strcat(string, "true}");
	else strcat(string, "false}");
	
	return string;
}

char* routeListToJSON(const List *list)
{
	if(list == NULL) return "[]";
	if(list->length == 0) return "[]";
	
	char * string = new_string();
	
	strcat(string, "[");
	
	ListIterator itr = createIterator((List *)list);
	Route * rte = nextElement(&itr);
	
	
	while(rte != NULL)
	{
		strcat(string, routeToJSON(rte));
		
		if((rte = nextElement(&itr)) != NULL) strcat(string, ",");
	}
	
	strcat(string, "]");
	
	return string;
}

char* trackListToJSON(const List *list)
{
	if(list == NULL) return "[]";
	if(list->length == 0) return "[]";
	
	char * string = new_string();
	
	strcat(string, "[");
	
	ListIterator itr = createIterator((List *)list);
	Track * trk = nextElement(&itr);
	
	
	while(trk != NULL)
	{
		strcat(string, trackToJSON(trk));
		
		if((trk = nextElement(&itr)) != NULL) strcat(string, ",");
	}
	
	strcat(string, "]");
	
	return string;
}

char* GPXtoJSON(const GPXdoc* gpx)
{
	if(gpx == NULL) return "{}";
	
	char * string = new_string();
	char buffer[50];
	
	strcpy(string, "{\"version\":");
	snprintf(buffer, sizeof(buffer), "%.1f", gpx->version);
	strcat(string, buffer);
	
	strcat(string, ",\"creator\":\"");
	strcat(string, gpx->creator);
	
	strcat(string, "\",\"numWaypoints\":");
	snprintf(buffer, sizeof(buffer), "%d", getLength(gpx->waypoints));
	strcat(string, buffer);
	
	strcat(string, ",\"numRoutes\":");
	snprintf(buffer, sizeof(buffer), "%d", getLength(gpx->routes));
	strcat(string, buffer);
	
	strcat(string, ",\"numTracks\":");
	snprintf(buffer, sizeof(buffer), "%d", getLength(gpx->tracks));
	strcat(string, buffer);
	
	strcat(string, "}");
	
	return string;
	
}

void addWaypoint(Route *rt, Waypoint *pt)
{
	if(rt == NULL) return;
	if(pt == NULL) return;
	
	insertBack(rt->waypoints, pt);
}

void addRoute(GPXdoc* doc, Route* rt)
{
	if(doc == NULL) return;
	if(rt == NULL) return;
	
	insertBack(doc->routes, rt);
}


GPXdoc* JSONtoGPX(const char* gpxString)
{
	if(gpxString == NULL) return NULL;
	
	GPXdoc * gdoc = new_gpxdoc();
	
	char ** arr = new_array();
	char * temp = new_string();
	char buffer[10];
	
	strcpy(temp, (char *)gpxString);
	temp[strlen(temp) + 1] = '\0';
	//printf("\ttemp = %s\n", temp);
	
	int c = 0, i = 0, quote = 0, start = 0, end = 0, j = 0;
	
	while(temp[i] != '\0')
	{
		if(temp[i] == '{')
		{
			//printf("found opening brace\n");
			i++;
			continue;
		}
	
		if(temp[i] == '\"')
		{
			//printf("found an opening quotation mark\n");
			quote = 1;
			start = i + 1;
			end = i + 1;
			i++;
	
			while(quote != 0)
			{
				while(temp[i] != '\"')
				{
					end++;
					i++;
					continue;
				}
			
				//printf("found an closing quotation mark\n");
				quote = 0;
			}

			custom_strcpy(arr[c], temp, start, end);
			//printf("the word is %s at index %d\n\n", arr[c], c);
		}
	
		if(temp[i] == ':')
		{
			
		  if(temp[i + 1] != '\"')
		  {
			while(temp[i + 1] != ',')
			{
				buffer[j] = temp[i + 1];
				//printf("I saw %c  and i point next at %c\n", temp[i], temp[i + 1]);
				j++;
				i++;
			}
		  }

		  i++;
		  c++;
		  continue;
		}
		
		
	
		c++;
		i++;
	}
	
	c = 0;
	
	//printf("\t\t%s\n", arr[0]);
	//printf("\t\t%s\n", arr[3]);
	//printf("\t\t%s\n", arr[5]);
	//printf("\t\t%s\n", buffer);
	

	if(strcmp("version", arr[0]) == 0)
	{
		gdoc->version = atof(buffer);
	}
	
	if(strcmp("creator", arr[3]) == 0)
	{
		strcpy(gdoc->creator, arr[5]);
	}
	
	strcpy(gdoc->namespace,  "http://www.topografix.com/GPX/1/1");
	
	gdoc->waypoints->length = 0;
	gdoc->routes->length = 0;
	gdoc->tracks->length = 0;
	
	free_string((void *)temp);
	free_array((void **)arr);
	
	return gdoc;
	
}


Waypoint* JSONtoWaypoint(const char* gpxString)
{
	if(gpxString == NULL) return NULL;
	
	Waypoint * wpt = new_waypoint();
	
	char ** arr = new_array();
	char * temp = new_string();
	char buffer[10];
	
	strcpy(temp, (char *)gpxString);
	temp[strlen(temp) + 1] = '\0';
	//printf("\ttemp = %s\n", temp);
	
	int c = 0, i = 0, quote = 0, start = 0, end = 0;
	
	while(temp[i] != '\0')
	{
		if(temp[i] == '{')
		{
			//printf("found opening brace\n");
			i++;
			continue;
		}
	
		if(temp[i] == '\"')
		{
			//printf("found an opening quotation mark\n");
			quote = 1;
			start = i + 1;
			end = i + 1;
			i++;
	
			while(quote != 0)
			{
				while(temp[i] != '\"')
				{
					end++;
					i++;
					continue;
				}
			
				//printf("found an closing quotation mark\n");
				quote = 0;
			}

			custom_strcpy(arr[c], temp, start, end);
			//printf("the word is %s at index %d and i = %d\n\n", arr[c], c, i);
		}
	
		c++;
		i++;
	}
	
	c = 0;
	
	if(strcmp(arr[0], "lat") == 0)
	{
		custom_strcpy(buffer, temp, 7, 16);
		buffer[9] = '\0';
		//printf("%s\n", buffer);
		wpt->latitude = atof(buffer);
	}
	
	if(strcmp(arr[12], "lon") == 0)
	{
		custom_strcpy(buffer, temp, 23, 34);
		buffer[10] = '\0';
		//printf("%s\n", buffer);
		wpt->longitude = atof(buffer);
	}

	
	wpt->attributes->length = 0;
	
	free_string((void *)temp);
	free_array((void **)arr);
	
	return wpt;
}

Route* JSONtoRoute(const char* gpxString)
{
	
	if(gpxString == NULL) return NULL;
	
	Route * rte = new_route();
	
	char ** arr = new_array();
	char * temp = new_string();
	
	strcpy(temp, (char *)gpxString);
	temp[strlen(temp) + 1] = '\0';
	//printf("\ttemp = %s\n", temp);
	
	int c = 0, i = 0, quote = 0, start = 0, end = 0;
	
	while(temp[i] != '\0')
	{
		if(temp[i] == '{')
		{
			//printf("found opening brace\n");
			i++;
			continue;
		}
	
		if(temp[i] == '\"')
		{
			//printf("found an opening quotation mark\n");
			quote = 1;
			start = i + 1;
			end = i + 1;
			i++;
	
			while(quote != 0)
			{
				while(temp[i] != '\"')
				{
					end++;
					i++;
					continue;
				}
			
				//printf("found an closing quotation mark\n");
				quote = 0;
			}

			custom_strcpy(arr[c], temp, start, end);
			//printf("the word is %s at index %d\n\n", arr[c], c);
		}
	
		c++;
		i++;
	}
	
	if(strcmp("name", arr[0]) == 0)
	{
		strcpy(rte->name, arr[2]);
	}
	
	rte->waypoints->length = 0;
	rte->attributes->length = 0;
	
	free_string((void *)temp);
	free_array((void **)arr);
	
	return rte;
}


