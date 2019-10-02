#include "helper.h"

GPXdoc * new_gpxdoc()
{
    GPXdoc * gdoc = malloc(sizeof(GPXdoc));
    gdoc->version = 1.1;
    gdoc->creator = new_string();
    
    char* (*rteToStr)(void*) = &routeToString;
	void (*delRte)(void*) = &deleteRoute;
	int (*compRte)(void*) = &compareRoutes;
    
    gdoc->routes = initializeList(rteToStr, delRte, compRte);
    
    char* (*wpToStr)(void*) = &waypointToString;
	void (*delWp)(void*) = &deleteWaypoint;
	int (*compWp)(void*) = &compareWaypoints;
    
    gdoc->waypoints = initializeList(wpToStr, delWp, compWp);
    
    char* (*trToStr)(void*) = &trackToString;
	void (*delTr)(void*) = &deleteTrack;
	int (*compTr)(void*) = &compareTracks;
    
    gdoc->tracks = initializeList(trToStr, delTr, compTr);
    
    return gdoc;
}

Attribute * new_attribute()
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
	int (*compAttr)(void*) = &compareAttributes;
	
	wp->attributes = initializeList(attToStr, delAttr, compAttr);
	
	return wp;
}

Route * new_route(void)
{
	Route * rte = malloc(sizeof(Route));
	rte->name = new_string();
	
	char* (*wpToStr)(void*) = &waypointToString;
	void (*delWp)(void*) = &deleteWaypoint;
	int (*compWp)(void*) = &compareWaypoints;
	
	rte->waypoints = initializeList(wpToStr, delWp, compWp);
	
	char* (*attrToStr)(void*) = &attributeToString;
	void (*delAttr)(void*) = &deleteAttribute;
	int (*compAttr)(void*) = &compareAttributes;
	
	rte->attributes = initializeList(attrToStr, delAttr, compAttr);
	
	return rte;
}

TrackSegment * new_tracksegment(void)
{
	TrackSegment * ts = malloc(sizeof(TrackSegment));
	
	char* (*attrToStr)(void*) = &attributeToString;
	void (*delAttr)(void*) = &deleteAttribute;
	int (*compAttr)(void*) = &compareAttributes;
	
	ts->attributes = initializeList(attrToStr, delAttr, compAttr);
	
	return ts;
}

Track * new_track(void)
{
	Track * tr = malloc(sizeof(Track));
	tr->name = new_string();
	
	char* (*tsToStr)(void*) = &trackSegmentToString;
	void (*delTs)(void*) = &deleteTrackSegment;
	int (*compTs)(void*) = &compareTrackSegments;
	
	tr->segments = initializeList(tsToStr, delTs,compTs);
	
	char* (*attrToStr)(void*) = &attributeToString;
	void (*delAttr)(void*) = &deleteAttribute;
	int (*compAttr)(void*) = &compareAttributes;
	
	tr->attributes = initializeList(attrToStr, delAttr, compAttr);
	
	return tr;
}

char * new_string(void)
{
	char * string = calloc(50, sizeof(char));
	
	return string;
}

void free_string(char * toFree)
{
	free(toFree);
}

int compare_strings(const void * first, const void * second)
{
	const char * a_first = (const char *) first;
	const char * b_second = (const char *) second;
	
	return strcmp(a_first, b_second);
}

void create_doc_from_tree(xmlDoc * doc, GPX ** gdoc)
{
	xmlNode * root = NULL;
	root = xmlDocGetRootElement(doc);
	
    mlNode *cur_node = NULL;

    for (cur_node = root; cur_node != NULL; cur_node = cur_node->next) {
    {
        if (cur_node->type == XML_ELEMENT_NODE) {
            printf("node type: Element, name: %s\n", cur_node->name);
            
            if(strcmp(cur_node->name, "gpx") == 0) 
            {
				//go to its attributes
				//assign version, creator, etc
			}
			
			if(strcmp(cur_node->name, "route") == 0) 
            {
				//go to its attributes
				//assign all that needs to be
			}
			
			if(strcmp(cur_node->name, "waypoint") == 0) 
            {
				//go to its attributes
				//assign all that needs to be
			}
			
			if(strcmp(cur_node->name, "track") == 0) 
            {
				//go to its attributes
				//assign all that needs to be
			}
        }
	}

         if (cur_node->content != NULL ){
             printf("  content: %s\n", cur_node->content);
         }

        xmlAttr *attr;
        
        for (attr = cur_node->properties; attr != NULL; attr = attr->next)
        {
            xmlNode *value = attr->children;
            char *attrName = (char *)attr->name;
            char *cont = (char *)(value->content);
            printf("\tattribute name: %s, attribute value = %s\n", attrName, cont);
        }

        create_doc_from_tree(cur_node->children, gdoc);
    }
}



