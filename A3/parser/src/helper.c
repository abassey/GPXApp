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
					
					for (attr = cur_node->properties; attr != NULL; attr = attr->next)
						{
							xmlNode *value = attr->children;
							char *attrName = (char *)attr->name;
							char *cont = (char *)(value->content);
							//printf("\tattribute name: %s, attribute value = %s\n", attrName, cont);
							
							if(strcmp(attrName, "lat") == 0) wpt->latitude = atof(cont);
							else if(strcmp(attrName, "lon") == 0) wpt->longitude = atof(cont);
							else if(strcmp(attrName, "name") == 0) strcpy(wpt->name, cont);
							else {
								Attribute * wpt_attr = new_attribute();
								
								strcpy(wpt_attr->name,attrName);
								strcpy(wpt_attr->value, cont);
								
								insertBack(wpt->attributes, wpt_attr);
								
								//deleteAttribute(wpt_attr);
							}
							
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
					
					//printf("found a route\n");
				
					for(rte_node = cur_node->children->next; rte_node != NULL; rte_node = rte_node->next->next)
					{
						//printf("node type: Element, name: %s\n", rte_node->name);
						
						if(strcmp((char *)rte_node->name, "name") == 0) strcpy(rte->name, (char*)xmlNodeGetContent(rte_node));
						else if (strcmp((char *)rte_node->name, "rtept") == 0) 
						{
							Waypoint * rte_wp = new_waypoint();
							
							for (attr = rte_node->properties; attr != NULL; attr = attr->next)
							{
								xmlNode *value = attr->children;
								char *attrName = (char *)attr->name;
								char *cont = (char *)(value->content);
								//printf("\tattribute name: %s, attribute value = %s\n", attrName, cont);
								
								if(strcmp(attrName, "lat") == 0) rte_wp->latitude = atof(cont);
								else if(strcmp(attrName, "lon") == 0) rte_wp->longitude = atof(cont);
								else if(strcmp(attrName, "name") == 0) strcpy(rte_wp->name, cont);
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
						//printf("node type: Element, name: %s\n", trk_node->name); 
						
						if(strcmp((char *)trk_node->name, "name") == 0) strcpy(trk->name, (char*)xmlNodeGetContent(trk_node));
						else if (strcmp((char *)trk_node->name, "trkseg") == 0)
						{
							TrackSegment* trkseg = new_tracksegment();
							
							//xmlNode * trksegs = NULL;
							Waypoint * trkseg_wp = new_waypoint();
							
							for (attr = trk_node->properties; attr != NULL; attr = attr->next)
							{
								xmlNode *value = attr->children;
								char *attrName = (char *)attr->name;
								char *cont = (char *)(value->content);
								//printf("\tattribute name: %s, attribute value = %s\n", attrName, cont);
								
								if(strcmp(attrName, "lat") == 0) trkseg_wp->latitude = atof(cont);
								else if(strcmp(attrName, "lon") == 0) trkseg_wp->longitude = atof(cont);
								else if(strcmp(attrName, "name") == 0) strcpy(trkseg_wp->name, cont);
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



