#include "GPXParser.h"
#include "helper.h"
#include "LinkedListAPI.h"
#include <stdio.h>

char * new_string(void)
{
	char * string = calloc(5000, sizeof(char));
	
	return string;
}

GPXdoc * new_gpxdoc(void)
{
    GPXdoc * gdoc = calloc(1, sizeof(GPXdoc));
    gdoc->version = 1.1;
    gdoc->creator = new_string();
    
    char* (*rteToStr)(void*) = &routeToString;
	void (*delRte)(void*) = &deleteRoute;
	int (*compRte)(const void*, const void*) = &compareRoutes;
    
    gdoc->routes = initializeList(rteToStr, delRte, compRte);
    
    char* (*wpToStr)(void*) = &waypointToString;
	void (*delWp)(void*) = &deleteWaypoint;
	int (*compWp)(const void*, const void*) = &compareWaypoints;
    
    gdoc->waypoints = initializeList(wpToStr, delWp, compWp);
    
    char* (*trToStr)(void*) = &trackToString;
	void (*delTr)(void*) = &deleteTrack;
	int (*compTr)(const void*, const void*) = &compareTracks;
    
    gdoc->tracks = initializeList(trToStr, delTr, compTr);
    
    return gdoc;
}

Attribute * new_attribute(void)
{
	Attribute * att = malloc(sizeof(Attribute));
	att->name = new_string();
	att->value = new_string();
	
	return att;
}

Waypoint * new_waypoint(void)
{
	Waypoint * wp = malloc(sizeof(Waypoint));
	wp->name = new_string();
	wp->longitude = 0.0;
	wp->latitude = 0.0;
	
	char* (*attrToStr)(void*) = &attributeToString; //fuction pointer. make for each one
	void (*delAttr)(void*) = &deleteAttribute;
	int (*compAttr)(const void*, const void*) = &compareAttributes;
	
	wp->attributes = initializeList(attrToStr, delAttr, compAttr);
	
	return wp;
}

Route * new_route(void)
{
	Route * rte = malloc(sizeof(Route));
	rte->name = new_string();
	
	char* (*wpToStr)(void*) = &waypointToString;
	void (*delWp)(void*) = &deleteWaypoint;
	int (*compWp)(const void*, const void*) = &compareWaypoints;
	
	rte->waypoints = initializeList(wpToStr, delWp, compWp);
	
	char* (*attrToStr)(void*) = &attributeToString;
	void (*delAttr)(void*) = &deleteAttribute;
	int (*compAttr)(const void*, const void*) = &compareAttributes;
	
	rte->attributes = initializeList(attrToStr, delAttr, compAttr);
	
	return rte;
}

TrackSegment * new_tracksegment(void)
{
	TrackSegment * ts = malloc(sizeof(TrackSegment));
	
	char* (*wpToStr)(void*) = &waypointToString;
	void (*delWp)(void*) = &deleteWaypoint;
	int (*compWp)(const void*, const void*) = &compareWaypoints;
	
	ts->waypoints = initializeList(wpToStr, delWp, compWp);
	
	return ts;
}

Track * new_track(void)
{
	Track * tr = malloc(sizeof(Track));
	tr->name = new_string();
	
	char* (*tsToStr)(void*) = &trackSegmentToString;
	void (*delTs)(void*) = &deleteTrackSegment;
	int (*compTs)(const void*, const void*) = &compareTrackSegments;
	
	tr->segments = initializeList(tsToStr, delTs,compTs);
	
	char* (*attrToStr)(void*) = &attributeToString;
	void (*delAttr)(void*) = &deleteAttribute;
	int (*compAttr)(const void*, const void*) = &compareAttributes;
	
	tr->attributes = initializeList(attrToStr, delAttr, compAttr);
	
	return tr;
}

void free_string(void * toFree)
{
	if(toFree == NULL) return;
	
	free(toFree);
}

int compare_strings(const void * first, const void * second)
{
	const char * a_first = (const char *) first;
	const char * b_second = (const char *) second;
	
	return strcmp(a_first, b_second);
}

void create_doc_from_tree(xmlNode * root, GPXdoc ** gdoc)
{
	//printf("in create doc function\n");
	
	if(root == NULL) return;
	
	xmlAttr *attr;
	
	GPXdoc * gpx = *gdoc;
	
    xmlNode *cur_node = NULL;

    for (cur_node = root; cur_node != NULL; cur_node = cur_node->next)
     {
		{
			if (cur_node->type == XML_ELEMENT_NODE) {
				
				//check for gpx tag
				if(strcmp((char *)cur_node->name, "gpx") == 0) 
				{
					//printf("node type: Element, name: %s\n", cur_node->name);
					strcpy(gpx->namespace, (char *)root->ns->href);
					//go to its attributes
					//assign version, creator, etc
					for (attr = cur_node->properties; attr != NULL; attr = attr->next)
					{
						xmlNode *value = attr->children;
						char *attrName = (char *)attr->name;
						char *cont = (char *)(value->content);
						//printf("\tattribute name: %s, attribute value = %s\n", attrName, cont);
						
						if(strcmp(attrName, "version") == 0) gpx->version = atof(cont);
						else if(strcmp(attrName, "creator") == 0) strcpy(gpx->creator, cont);
						 //come back to this. not copying properly
						
					}
				}
				
				//check for waypoint tag
				if(strcmp((char *)cur_node->name, "wpt") == 0) 
				{
					//printf("node type: Element, name: %s\n", cur_node->name);
					//printf("hello!\n\n");
					//go to its attributes
					//assign all that needs to be
					Waypoint * wpt = new_waypoint();
				
					if(cur_node->children != NULL)
					{
						xmlNode * wpt_node = NULL;
						
						for(wpt_node = cur_node->children->next; wpt_node != NULL; wpt_node = wpt_node->next->next)
						{
						
							if(strcmp((char *)wpt_node->name, "name") == 0) strcpy(wpt->name, (char*)xmlNodeGetContent(wpt_node));
						}
					}
					
					for (attr = cur_node->properties; attr != NULL; attr = attr->next)
						{
							//incorrectvalues are being stored into lat and lon. look into it
							xmlNode *value = attr->children;
							char *attrName = (char *)attr->name;
							char *cont = (char *)(value->content);
							//printf("\twaypoint attribute name: %s, waypoint attribute value = %s\n", attrName, cont);
							
							if(strcmp(attrName, "lat") == 0) wpt->latitude = atof(cont);
							else if(strcmp(attrName, "lon") == 0) wpt->longitude = atof(cont);
							else {
								Attribute * wpt_attr = new_attribute();
								
								strcpy(wpt_attr->name,attrName);
								strcpy(wpt_attr->value, cont);
								
								insertBack(wpt->attributes, wpt_attr);
								
								//deleteAttribute(wpt_attr);
							}
							
							//printf("copied lat:%f, copied lon:%f\n", wpt->latitude, wpt->longitude);
						}
					
					insertBack(gpx->waypoints, wpt);
					//deleteWaypoint(wpt);
					
				}
			
			//check for route tag
			if(strcmp((char*)cur_node->name, "rte") == 0) 
				{
					Route * rte = new_route();
					
					//go to its attributes
					//assign all that needs to be
					xmlNode * rte_node = NULL; 
				
					for(rte_node = cur_node->children->next; rte_node != NULL; rte_node = rte_node->next->next)
					{
						//printf("node type: Element, name: %s\n", rte_node->name);
						
						if(strcmp((char *)rte_node->name, "name") == 0) strcpy(rte->name, (char*)xmlNodeGetContent(rte_node));
						else if (strcmp((char *)rte_node->name, "rtept") == 0) 
						{
							Waypoint * rte_wp = new_waypoint();
							
							
							if(rte_node->children != NULL)
								{
									xmlNode * rtept_node = NULL;
									
									for(rtept_node = rte_node->children->next; rtept_node != NULL; rtept_node = rtept_node->next->next)
									{
										if(strcmp((char *)rtept_node->name, "name") == 0) strcpy(rte_wp->name, (char*)xmlNodeGetContent(rtept_node));
										
										if(strcmp((char *)rtept_node->name, "ele") == 0)
										{
											Attribute * rte_attr = new_attribute();
													
											strcpy(rte_attr->name, "ele");
											strcpy(rte_attr->value, (char*)xmlNodeGetContent(rtept_node));
													
											insertBack(rte_wp->attributes, rte_attr);
										} 
									}
								} else {
								}
							
							for (attr = rte_node->properties; attr != NULL; attr = attr->next)
							{
								xmlNode *value = attr->children;
								char *attrName = (char *)attr->name;
								char *cont = (char *)(value->content);
								//printf("\tattribute name: %s, attribute value = %s\n", attrName, cont);
								
								if(strcmp(attrName, "lat") == 0) rte_wp->latitude = atof(cont);
								else if(strcmp(attrName, "lon") == 0) rte_wp->longitude = atof(cont);
								else {
									Attribute * rtept_attr = new_attribute();
									
									strcpy(rtept_attr->name,attrName);
									strcpy(rtept_attr->value, cont);
									
									insertBack(rte_wp->attributes, rtept_attr);
									
									//deleteAttribute(rtept_attr);
								}
								
							}
							
							insertBack(rte->waypoints, rte_wp);
							//deleteWaypoint(rte_wp);
						} else {
							Attribute * rte_attr = new_attribute();
									
							strcpy(rte_attr->name,(char *)rte_node->name);
							strcpy(rte_attr->value, (char*)xmlNodeGetContent(rte_node));
							
							insertBack(rte->attributes, rte_attr);
							
							//deleteAttribute(rte_attr);
						}
						
					}
					
					insertBack(gpx->routes, rte);
						//deleteRoute(rte);
				}
				
				
				
				//check for track tag
				if(strcmp((char*)cur_node->name, "trk") == 0) 
				{
					//go to its attributes
					//assign all that needs to be
					Track * trk = new_track();
					
					xmlNode * trk_node = NULL;
					
					for(trk_node = cur_node->children->next; trk_node != NULL; trk_node = trk_node->next->next)
					{
						
						if(strcmp((char *)trk_node->name, "name") == 0) strcpy(trk->name, (char*)xmlNodeGetContent(trk_node));
						else if (strcmp((char *)trk_node->name, "trkseg") == 0)
						{
							TrackSegment* trkseg = new_tracksegment();
							
							xmlNode * trkseg_node = NULL;
							for(trkseg_node = trk_node->children->next; trkseg_node != NULL; trkseg_node = trkseg_node->next->next)
							{
								//printf("node type: Element, name: %s\n", trkseg_node->name); 
									
								Waypoint * trkseg_wp = new_waypoint();
								
								if(trkseg_node->children != NULL)
								{
									xmlNode * trkpt_node = NULL;
									
									for(trkpt_node = trkseg_node->children->next; trkpt_node != NULL; trkpt_node = trkpt_node->next->next)
									{
										if(strcmp((char *)trkpt_node->name, "name") == 0) strcpy(trkseg_wp->name, (char*)xmlNodeGetContent(trkpt_node));
										
										if(strcmp((char *)trkpt_node->name, "ele") == 0)
										{
											Attribute * trkseg_attr = new_attribute();
													
											strcpy(trkseg_attr->name, "ele");
											strcpy(trkseg_attr->value, (char*)xmlNodeGetContent(trkpt_node));
													
											insertBack(trkseg_wp->attributes, trkseg_attr);
										} 
									}
								} else {
								}
								
								for (attr = trkseg_node->properties; attr != NULL; attr = attr->next)
								{
									xmlNode *value = attr->children;
									char *attrName = (char *)attr->name;
									char *cont = (char *)(value->content);
									//printf("\tattribute name: %s, attribute value = %s\n", attrName, cont);
									
									if(strcmp(attrName, "lat") == 0) trkseg_wp->latitude = atof(cont);
									else if(strcmp(attrName, "lon") == 0) trkseg_wp->longitude = atof(cont);
									else {
										Attribute * trkseg_attr = new_attribute();
										
										strcpy(trkseg_attr->name,attrName);
										strcpy(trkseg_attr->value, cont);
										
										insertBack(trkseg_wp->attributes, trkseg_attr);
										
										//deleteAttribute(trkseg_attr);
									}
									
								}
								
								insertBack(trkseg->waypoints, trkseg_wp);
								//deleteWaypoint(trkseg_wp);
							}
							
							insertBack(trk->segments, trkseg);
							//deleteTrackSegment(trkseg);
						
						} else {
							Attribute * trk_attr = new_attribute();
									
							strcpy(trk_attr->name,(char *)trk_node->name);
							strcpy(trk_attr->value, (char*)xmlNodeGetContent(trk_node));
							
							insertBack(trk->attributes, trk_attr);
							
							//deleteAttribute(trk_attr);
						}
					}
						
						insertBack(gpx->tracks, trk);
						//deleteTrack(trk);
				}
			} 

		}

        create_doc_from_tree(cur_node->children, gdoc);
    }
    
    *gdoc = gpx;
}

//Assignment 2 start
void create_tree_from_doc(xmlNodePtr * root, GPXdoc * gdoc)
{
	xmlNodePtr root_node = *root;
	xmlNodePtr node = NULL;
	xmlNodePtr child = NULL;
	xmlNodePtr inner_child = NULL;
	char float_str[20];
	
	snprintf(float_str, sizeof(float_str), "%.1f", gdoc->version);
	xmlNewProp(root_node, BAD_CAST "version", BAD_CAST float_str);
	xmlNewProp(root_node, BAD_CAST "creator", BAD_CAST gdoc->creator);
	xmlNsPtr ns = xmlNewNs(root_node, BAD_CAST gdoc->namespace, NULL);
	xmlSetNs(root_node, ns);
	
	
	//creating the waypoint child node, its attributes and its children
	if(getLength(gdoc->waypoints) != 0)
	{
		ListIterator itr = createIterator(gdoc->waypoints);
		
		Waypoint * wp  = nextElement(&itr);
		
		while(wp != NULL)
		{
			node = xmlNewChild(root_node, NULL, BAD_CAST "wpt", NULL);
		
			if(getLength(wp->attributes) != 0)
			{	
				ListIterator inner_itr = createIterator(wp->attributes);
				
				Attribute * attr_data = nextElement(&inner_itr);
				
				while(attr_data != NULL)
				{
					if (strcmp(attr_data->name, "ele") == 0) xmlNewChild(node, NULL, BAD_CAST "ele", BAD_CAST attr_data->value);
					else xmlNewProp(node, BAD_CAST attr_data->name, BAD_CAST attr_data->value);
						
					
					attr_data = nextElement(&inner_itr);
				}
				
				//deleteAttribute(attr);
			}
			
			//child = xmlNewChild(node, BAD_CAST "name", BAD_CAST wp->name);
			if(strcmp(wp->name, "\0") != 0) xmlNewChild(node, NULL, BAD_CAST "name", BAD_CAST wp->name);
			
			snprintf(float_str, sizeof(float_str), "%.6f", wp->latitude);
			xmlNewProp(node, BAD_CAST "lat", BAD_CAST float_str);
			
			snprintf(float_str, sizeof(float_str), "%.6f", wp->longitude);
			xmlNewProp(node, BAD_CAST "lon", BAD_CAST float_str);
			
			wp = nextElement(&itr);
		}
	}
	
	//creating the route child node, its attributes and its children
	if(getLength(gdoc->routes) != 0)
	{
		
		
		ListIterator itr = createIterator(gdoc->routes);
		
		Route * rte  = nextElement(&itr);
		
		while(rte != NULL)
		{
			node = xmlNewChild(root_node, NULL, BAD_CAST "rte", NULL);
			
			if(strcmp(rte->name, "\0") != 0) xmlNewChild(node, NULL, BAD_CAST "name", BAD_CAST rte->name);
		
			if(getLength(rte->attributes) != 0)
			{
				
				ListIterator inner_itr = createIterator(rte->attributes);
				
				Attribute * attr = nextElement(&inner_itr);
				
				while(attr != NULL)
				{
					if(strcmp(attr->name,"desc") == 0) 
					child = xmlNewChild(node, NULL, BAD_CAST attr->name, BAD_CAST attr->value);
					
					else
					xmlNewProp(node, BAD_CAST attr->name, BAD_CAST attr->value);
					
					attr = nextElement(&inner_itr);
				}
				
			}
			
			
			if(getLength(rte->waypoints) != 0)
			{
				ListIterator rtewp_itr = createIterator(rte->waypoints);
				
				Waypoint * rte_wp = nextElement(&rtewp_itr);
				
				while(rte_wp != NULL)
				{
					child = xmlNewChild(node, NULL, BAD_CAST "rtept", NULL);
					
					if(strcmp(rte_wp->name, "\0") != 0) xmlNewChild(child, NULL, BAD_CAST "name", BAD_CAST rte_wp->name);
					//xmlNewProp(child, BAD_CAST "name", BAD_CAST rte_wp->name);
					
					snprintf(float_str, sizeof(float_str), "%.6f", rte_wp->latitude);
					xmlNewProp(child, BAD_CAST "lat", BAD_CAST float_str);
					
					snprintf(float_str, sizeof(float_str), "%.6f", rte_wp->longitude);
					xmlNewProp(child, BAD_CAST "lon", BAD_CAST float_str);
					
					ListIterator rtewp_attr_itr = createIterator(rte_wp->attributes);
				
					Attribute * rtewp_attr = nextElement(&rtewp_attr_itr);
					
					while(rtewp_attr != NULL)
					{
						xmlNewProp(child, BAD_CAST rtewp_attr->name, BAD_CAST rtewp_attr->value);
					
						rtewp_attr = nextElement(&rtewp_attr_itr);
					}
					
					rte_wp = nextElement(&rtewp_itr);
				}
			}
			
			rte = nextElement(&itr);
		}
	}
	
	if(getLength(gdoc->tracks) != 0)
	{
		//creating the track child node, its attributes and its children
		ListIterator itr = createIterator(gdoc->tracks);
		
		Track * trk  = nextElement(&itr);
		
		while(trk != NULL)
		{
			node = xmlNewChild(root_node, NULL, BAD_CAST "trk", NULL);
			
			child = xmlNewChild(node, NULL, BAD_CAST "name", BAD_CAST trk->name);
		
			if(getLength(trk->attributes) != 0)
			{
				
				ListIterator inner_itr = createIterator(trk->attributes);
				
				Attribute * attr = nextElement(&inner_itr);
				
				while(attr != NULL)
				{
					if(strcmp(attr->name,"desc") == 0) 
					child = xmlNewChild(node, NULL, BAD_CAST attr->name, BAD_CAST attr->value);
					
					else
					xmlNewProp(node, BAD_CAST attr->name, BAD_CAST attr->value);
					
					attr = nextElement(&inner_itr);
				}
				
			}
			
			
			if(getLength(trk->segments) != 0)
			{
				ListIterator trkseg_itr = createIterator(trk->segments);
				
				TrackSegment * trkseg = nextElement(&trkseg_itr);
				
				while(trkseg != NULL)
				{
					child = xmlNewChild(node, NULL, BAD_CAST "trkseg", NULL);
					
					ListIterator trksegwp_itr = createIterator(trkseg->waypoints);
				
					Waypoint * trkseg_wp = nextElement(&trksegwp_itr);
				
					while(trkseg_wp != NULL)
					{
						
						inner_child = xmlNewChild(child, NULL, BAD_CAST "trkpt", NULL);
						
						if(strcmp(trkseg_wp->name, "\0") != 0) xmlNewChild(inner_child, NULL, BAD_CAST "name", BAD_CAST trkseg_wp->name);
						//xmlNewProp(inner_child, BAD_CAST "name", BAD_CAST trkseg_wp->name);
						
						snprintf(float_str, sizeof(float_str), "%.6f", trkseg_wp->latitude);
						xmlNewProp(inner_child, BAD_CAST "lat", BAD_CAST float_str);
						
						snprintf(float_str, sizeof(float_str), "%.6f", trkseg_wp->longitude);
						xmlNewProp(inner_child, BAD_CAST "lon", BAD_CAST float_str);
						
						ListIterator trksegwp_attr_itr = createIterator(trkseg_wp->attributes);
					
						Attribute * trksegwp_attr = nextElement(&trksegwp_attr_itr);
						
						while(trksegwp_attr != NULL)
						{
							if(strcmp(trksegwp_attr->name,"ele") == 0) 
							xmlNewChild(inner_child, NULL, BAD_CAST trksegwp_attr->name, BAD_CAST trksegwp_attr->value);
							
							else
							xmlNewProp(inner_child, BAD_CAST trksegwp_attr->name, BAD_CAST trksegwp_attr->value);
						
							trksegwp_attr = nextElement(&trksegwp_attr_itr);
						}
						
						trkseg_wp = nextElement(&trksegwp_itr);
					}
					
					trkseg = nextElement(&trkseg_itr);
				}
			}
				
				trk = nextElement(&itr);
		}
	}
	
	*root = root_node;
}

float dist_calc(float lat1, float lon1, float lat2, float lon2)
{
	
	float dx, dy, dz;
	lon1 -= lon2;
	lon1 *= TO_RAD, lat1 *= TO_RAD, lat2 *= TO_RAD;
 
	dz = sinf(lat1) - sinf(lat2);
	dx = cosf(lon1) * cosf(lat1) - cosf(lat2);
	dy = sinf(lon1) * cosf(lat1);
	return asinf(sqrtf(dx * dx + dy * dy + dz * dz) / 2) * 2 * R;
}

void free_array(void ** arr)
{
  if(arr == NULL) return;

  for(int i = 0; i < 5000; i++)
  {
    free_string(arr[i]);
  }

  free(arr);
}

char ** new_array(void)
{
  char ** allocate = calloc(5000, sizeof(char*));

  for(int i = 0; i < 5000; i++)
  {
    allocate[i] = calloc(5000, sizeof(char));
  }

  return allocate;
}

void custom_strcpy(char * dest, char * scr, int start_index, int end_index)
{
  if(dest == NULL || scr == NULL) return;

  if(start_index < 0) return;

  if(end_index > strlen(scr)) return;

  int counter = 0;

  for(int i = start_index; i < end_index; i++)
  {
    dest[counter] = scr[i];
    counter++;
  }
}

char * gpx_struct_to_html(char * filename)
{
	char * dir = new_string();
	strcpy(dir, "uploads/");
	strcat(dir, filename);
	
	GPXdoc * gdoc = createGPXdoc(dir);
	
	bool valid = validateGPXDoc(gdoc, "gpx.xsd");
	if(valid == false) return "Invalid file";
	
	char * str = GPXtoJSON(gdoc);
	
	deleteGPXdoc(gdoc);
	free_string(dir);
	
	return str;
}

char * trk_struct_to_html(char * filename)
{
	char * dir = new_string();
	strcpy(dir, "uploads/");
	strcat(dir, filename);
	
	GPXdoc * gdoc = createGPXdoc(dir);
	
	bool valid = validateGPXDoc(gdoc, "gpx.xsd");
	if(valid == false) return "Invalid track";
	
	char * str = trackListToJSON(gdoc->tracks);
	
	deleteGPXdoc(gdoc);
	free_string(dir);
	
	return str;
}

char * rte_struct_to_html(char * filename)
{
	char * dir = new_string();
	strcpy(dir, "uploads/");
	strcat(dir, filename);
	
	GPXdoc * gdoc = createGPXdoc(dir);
	
	bool valid = validateGPXDoc(gdoc, "gpx.xsd");
	if(valid == false) return "Invalid route";
	
	char * str = routeListToJSON(gdoc->routes);
	
	deleteGPXdoc(gdoc);
	free_string(dir);
	
	return str;
}

char * html_to_gpx_struct(char * filename, char * gpxJSON)
{
	char * dir = new_string();
	strcpy(dir, "uploads/");
	strcat(dir, filename);
	
	GPXdoc * gdoc = JSONtoGPX(gpxJSON);
	
	printf("infunc: %s\n", GPXdocToString(gdoc));
	printf("infunc: %s\n", dir);

	bool write = writeGPXdoc(gdoc, dir);
	if(write == false) return "Error writing to file";
	
	char * str = GPXtoJSON(gdoc);
	
	deleteGPXdoc(gdoc);
	free_string(dir);
	
	return str;
}

void rename_route(char * filename, char * newn, int index)
{
	char * dir = new_string();
	strcpy(dir, "uploads/");
	strcat(dir, filename);
	
	GPXdoc * gdoc = createGPXdoc(dir);

	bool valid = validateGPXDoc(gdoc, "gpx.xsd");
	if(valid == false) return;
	
	//change by index instead
	ListIterator itr = createIterator(gdoc->routes);

	Route * data = nextElement(&itr);
	int i = 0;
	while (data != NULL)
	{
		if(i == index) strcpy(data->name, newn);
		else i++;
		data = nextElement(&itr);
	}
	
	
	//printf("infunc: %s\n", newname->name);
	//addRoute(gdoc, newname);
	
	
	bool write = writeGPXdoc(gdoc, dir);
	if(write == false) return;
	
	
	deleteGPXdoc(gdoc);
	free_string(dir);
}

void rename_track(char * filename, char * newn, int index)
{
	char * dir = new_string();
	strcpy(dir, "uploads/");
	strcat(dir, filename);
	
	GPXdoc * gdoc = createGPXdoc(dir);

	bool valid = validateGPXDoc(gdoc, "gpx.xsd");
	if(valid == false) return;
	
	//change by index instead
	ListIterator itr = createIterator(gdoc->tracks);

	Track * data = nextElement(&itr);
	int i = 0;
	while (data != NULL)
	{
		if(i == index) strcpy(data->name, newn);
		else i++;
		data = nextElement(&itr);
	}
	
	
	bool write = writeGPXdoc(gdoc, dir);
	if(write == false) return;
	
	
	deleteGPXdoc(gdoc);
	free_string(dir);
}

char * all_rtes_between(char * filename, float start_lat, float start_long, float end_lat, float end_long, float tol)
{
	char * dir = new_string();
	strcpy(dir, "uploads/");
	strcat(dir, filename);
	
	GPXdoc * gdoc = createGPXdoc(dir);
	
	List * routes = getRoutesBetween(gdoc, start_lat, start_long, end_lat, end_long, tol);
	char * rtes = routeListToJSON(routes);
	
	deleteGPXdoc(gdoc);
	free_string(dir);
	
	return rtes;
}

char * all_trks_between(char * filename, float start_lat, float start_long, float end_lat, float end_long, float tol)
{
	char * dir = new_string();
	strcpy(dir, "uploads/");
	strcat(dir, filename);
	
	GPXdoc * gdoc = createGPXdoc(dir);
	
	List * tracks = getTracksBetween(gdoc, start_lat, start_long, end_lat, end_long, tol);
	char * trks = trackListToJSON(tracks);
	
	deleteGPXdoc(gdoc);
	free_string(dir);
	
	return trks;
}

void add_route_from_form(char * filename, char * route_name, char * waypoints, int length)
{
	char * dir = new_string();
	strcpy(dir, "uploads/");
	strcat(dir, filename);
	
	char ** wyps = new_array();
	char * temp = new_string();
	
	strcpy(temp, waypoints);
	temp[strlen(temp) + 1] = '\0';
	//printf("\ttemp = %s\n", temp);
	
	int c = 0, i = 0, brace = 0, start = 0, end = 0;
	
	while(temp[i] != ']')
	{
		if(temp[i] == '[')
		{
			//printf("found opening brace\n");
			i++;
			continue;
		}
		
		if(temp[i] == ',')
		{
			i++;
			continue;
		}
			
				if(temp[i] == '{')
				{
					//printf("found an opening quotation mark\n");
					brace = 1;
					start = i;
					end = i + 2;
					i++;
			
					while(brace != 0)
					{
						while(temp[i] != '}')
						{
							end++;
							i++;
							continue;
						}
					
						//printf("found an closing quotation mark\n");
						brace = 0;
					}

					custom_strcpy(wyps[c], temp, start, end);
		
					//printf("the word is %s\n\n", wyps[c]);
					c++;
				}
			
			i++;
		
	}
	
	
	GPXdoc * gdoc = createGPXdoc(dir);
	
	Route * rte = JSONtoRoute(route_name);
	
	for(int j = 0; j < length; j++)
	{
		Waypoint * wp = JSONtoWaypoint(wyps[j]);
		addWaypoint(rte, wp);
	}
	
	addRoute(gdoc, rte);
	
	bool write = writeGPXdoc(gdoc, dir);
	if(write == false) return;
	
	deleteGPXdoc(gdoc);
	free_string(dir);
	free_string(temp);
	free_array((void**)wyps);
}

char * points_struct_to_html(char * filename)
{
	char * dir = new_string();
	strcpy(dir, "uploads/");
	strcat(dir, filename);
	
	GPXdoc * gdoc = createGPXdoc(dir);
	
	bool valid = validateGPXDoc(gdoc, "gpx.xsd");
	if(valid == false) return "Invalid route";
	
	char * str = pointsListToJSON(gdoc->routes);
	printf("%s\n", str);
	
	deleteGPXdoc(gdoc);
	free_string(dir);
	
	return str;
}

char* pointsListToJSON(const List *list)
{
	if(list == NULL) return "[]";
	if(list->length == 0) return "[]";
	int i = 1;
	int j = 0;
	char buff[20];
	
	char * string = new_string();
	
	strcat(string, "[");
	
	ListIterator itr = createIterator((List *)list);
	Route * rte = nextElement(&itr);
	
	
	while(rte != NULL)
	{
		strcat(string, "{\"route_name\":\"");
		if(strcmp(rte->name, "\0") == 0) {
		strcat(string, "Unnamed ");
		sprintf(buff, "%d", i);
		strcat(string, buff);
		i++;
		}
		else strcat(string, rte->name);
		
		strcat(string, "\",\"point_info\":[");
		
		ListIterator itr2 = createIterator((List *)rte->waypoints);
		Waypoint * wp = nextElement(&itr2);
		
		while(wp != NULL)
		{
			strcat(string, "{\"point_index\":");
			sprintf(buff, "%d", j);
			strcat(string, buff);
			
			strcat(string, pointToJSON(wp));
			
			if((wp = nextElement(&itr2)) != NULL){
				strcat(string, ",");
				 j++;
				 }
		}
		
		strcat(string, "]}");
		if((rte = nextElement(&itr)) != NULL){
				 strcat(string, ",");
				 }
	}
	
	strcat(string, "]");
	
	return string;
}

char* pointToJSON(const Waypoint *wpt)
{
	if(wpt == NULL) return "{}";
	
	char * string = new_string();
	char buffer[50];
	
	strcat(string, ",\"name\":\"");
	if(strcmp(wpt->name, "\0") == 0) {
		strcat(string, "None");
		}
	else {
		strcat(string, wpt->name);
		}
	
	strcat(string, "\",\"lat\":");
	snprintf(buffer, sizeof(buffer), "%f", wpt->latitude);
	strcat(string, buffer);
	
	strcat(string, ",\"lon\":");
	snprintf(buffer, sizeof(buffer), "%f", wpt->longitude);
	strcat(string, buffer);
	strcat(string, "}");
	
	return string;
}
