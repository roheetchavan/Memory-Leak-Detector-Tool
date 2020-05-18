/*                                                                                                            
 * =====================================================================================                      
 *                                                                                                            
 *  Filename:  mldt.c                                                                                     
 *                                                                                                            
 *  Description:  Specifies the implementations
 *                                                                                                            
 *  This file is part of the Memery Leak Detector Tool distribution
 *  (https://github.com/roheetchavan).                          
 *  
 *  Copyright (c) 2020 Rohit Chavan. roheetchavan@gmail.com                                                                  
 *  This program is free software: you can redistribute it and/or modify                                
 *  it under the terms of the GNU General Public License as published by                                
 *  the Free Software Foundation, version 3.                                                            
 *                                                                                                            
 *  This program is distributed in the hope that it will be useful, but                                 
 *  WITHOUT ANY WARRANTY; without even the implied warranty of                                          
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU                                    
 *  General Public License for more details.                                                            
 *                                                                                                           
 *  You should have received a copy of the GNU General Public License                                   
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.                                
 *                                                                                                            
 * =====================================================================================                      
 */                                                            

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mldt.h"
#include "css.h"

char *DATA_TYPE[] = {"UINT8", "UINT32", "INT32",
					 "CHAR", "OBJ_PTR", "FLOAT",
					 "DOUBLE", "OBJ_STRUCT"};

void print_struct_record (struct_db_rec_t* struct_record) {
	int i = 0;
	field_info_t *field = NULL;

	if (!struct_record) return;
    
	printf(CYAN_COLOR "|------------------------------------------------------|\n" RESET_COLOR);
    printf(YELLOW_COLOR "| %-20s | size = %-8d | #flds = %-3d |\n" RESET_COLOR,
						 struct_record->name, struct_record->size, struct_record->nfields);
    printf(CYAN_COLOR "|------------------------------------------------------|-------------------------"
					  "-----------------------------------------------------------------|\n" RESET_COLOR);
    for(i = 0; i < struct_record->nfields; i++){
        field = &struct_record->fields[i];
        printf(CYAN_COLOR "  %-20s |" RESET_COLOR, "");
        printf("%-3d %-20s" CYAN_COLOR" |"RESET_COLOR" dtype = %-15s"CYAN_COLOR" |"RESET_COLOR
			   " size = %-5d"CYAN_COLOR" |"RESET_COLOR" offset = %-6d"CYAN_COLOR"|"RESET_COLOR
			   "  nstructname = %-20s  "CYAN_COLOR"|\n" RESET_COLOR, i, field->name, DATA_TYPE[field->dtype],
			    field->size, field->offset, field->nested_struct_name);
        printf(CYAN_COLOR "  %-20s |" RESET_COLOR, "");
        printf(CYAN_COLOR "------------------------------------------------------------------------------"
						  "--------------------------------------------|\n" RESET_COLOR);
    }
}

void print_struct_db (struct_db_t *struct_db) {
	int i = 0;
	
	if (!struct_db)	return;
	
	struct_db_rec_t *record = struct_db->head;

	printf("Printing structure database\n");
	printf("No of struct db records: %d\n", struct_db->count);
	while (record) {
		printf("Structure No: %d (%p)\n", i++, record);
		print_struct_record(record);
		record = record->next;
	}
}

int add_struct_to_struct_db(struct_db_t *struct_db, struct_db_rec_t *struct_record) {
	struct_db_rec_t *head = struct_db->head;
	
	if (!head) {
		struct_db->head = struct_record;
		struct_record->next = NULL;
		struct_db->count++;
		return 0;
	}
	
	struct_record->next = head;
	struct_db->head = struct_record;
	struct_db->count++;
	return 0;
}

struct_db_rec_t* struct_db_lookup(struct_db_t* struct_db, char *struct_name) {
	struct_db_rec_t *record = struct_db->head;

	if ((!record) || (!struct_db->count))	return NULL;

	while (record) {
		if (!strncmp(record->name, struct_name, sizeof(struct_name))) {
			return record;
		}
		record = record->next;
	}
	return NULL;
}

static object_db_rec_t* object_db_lookup(object_db_t* object_db, void *ptr) {
	object_db_rec_t* head = object_db->head;
	
	if (!head)	return NULL;
	
	while (head) {
		if (head->ptr == ptr)
			return head;
		head = head->next;
	}
	return NULL;
}

static void  add_object_to_object_db(object_db_t *object_db, void *ptr, int units,
									 struct_db_rec_t *struct_rec) {

	object_db_rec_t *obj_rec = object_db_lookup(object_db, ptr);
	
	/* Don't allow to insert same object twice */
	assert(!obj_rec);
	
	obj_rec = calloc(1, sizeof(object_db_rec_t));
	obj_rec->next = NULL;
	obj_rec->ptr = ptr;
	obj_rec->units = units;
	obj_rec->struct_rec = struct_rec;

	object_db_rec_t *head = object_db->head;
	
	if (!head) {
		object_db->head = obj_rec;
		obj_rec->next = NULL;
		object_db->count++;
		return;
	} 
	
	obj_rec->next = head;
	object_db->head = obj_rec;
	object_db->count++;
}

void *xmalloc(object_db_t *object_db, char *struct_name, int units) {
	struct_db_rec_t *struct_rec = struct_db_lookup(object_db->struct_db, struct_name);
	assert(struct_rec);
	void *ptr = calloc(units, struct_rec->size);
	add_object_to_object_db(object_db, ptr, units, struct_rec);
	return ptr;
}

void print_object_record(object_db_rec_t* object_rec) {
	if(!object_rec) return;                                                                                      
     
	printf(MAGENTA_COLOR"-----------------------------------------------------------------------------------|\n");
    printf(YELLOW_COLOR" ptr = %-10p | next = %-10p | units = %-4d | struct_name = %-10s |\n",      
          object_rec->ptr, object_rec->next, object_rec->units, object_rec->struct_rec->name);                    
    printf(MAGENTA_COLOR"-----------------------------------------------------------------------------------|\n");	
}

void print_object_db(object_db_t* object_db) {
	int i = 0;
	
	if (!object_db)	return;
	
	object_db_rec_t *record = object_db->head;

	printf("Printing object database\n");
	printf("No of object db records: %d\n", object_db->count);
	while (record) {
		printf("Object No: %d \n", i++);
		print_object_record(record);
		printf("Object details:\n");
		print_object_record_details(record);
		record = record->next;
	}
}


void print_object_record_details(object_db_rec_t *obj_rec) {
	int no_units = obj_rec->units;
	int no_fields = obj_rec->struct_rec->nfields;

	int obj_index, field_index;
	field_info_t *field_info = NULL;
	
	for (obj_index = 0 ; obj_index < no_units ; obj_index++) {
		char *curr_obj_ptr = (char*)(obj_rec->ptr) + (obj_index * obj_rec->struct_rec->size);

		for (field_index = 0; field_index < no_fields; field_index++) {
			field_info = &obj_rec->struct_rec->fields[field_index];
			
			switch (field_info->dtype) {
			    case UINT_8		:                                                                                   
                case INT_32		:                                                                                   
                case UINT_32	:                                                                                  
                     printf("%s[%d]->%s = %d\n", obj_rec->struct_rec->name, obj_index, field_info->name, *(int *)(curr_obj_ptr + field_info->offset));
                     break;                                                                                    
                case CHAR		:                                                                                    
                     printf("%s[%d]->%s = %s\n", obj_rec->struct_rec->name, obj_index, field_info->name, (char *)(curr_obj_ptr + field_info->offset));
                     break;                                                                                    
                case FLOAT		:                                                                                   
                     printf("%s[%d]->%s = %f\n", obj_rec->struct_rec->name, obj_index, field_info->name, *(float *)(curr_obj_ptr + field_info->offset));
                     break;                                                                                    
                case DOUBLE		:                                                                                  
                     printf("%s[%d]->%s = %f\n", obj_rec->struct_rec->name, obj_index, field_info->name, *(double *)(curr_obj_ptr + field_info->offset));
                     break;                                                                                    
                case OBJ_PTR	:                                                                                 
                     printf("%s[%d]->%s = %p\n", obj_rec->struct_rec->name, obj_index, field_info->name,  (void *)*(int *)(curr_obj_ptr + field_info->offset));
                     break;                                                                                    
                case OBJ_STRUCT	:                                                                              
                     /*Later*/                                                                                 
                     break;                                                                                    
                default	:                                                                                      
                     break;
		    }
		}
		printf("---------------------------------------\n");
	}	
}

static void delete_record_from_object_db(object_db_t* obj_db, object_db_rec_t* obj_rec) {
	
	if (!obj_rec)	return;

	object_db_rec_t* head = obj_db->head;
	if (head == obj_rec) {
		obj_db->head = obj_rec->next;
		free(obj_rec);
		obj_db->count--;
		return;
	} 

	object_db_rec_t* prev = obj_db->head;
	while (head) {
		if (head == obj_rec)
			break;
		prev = head;
		head = head->next;
	}

	if (prev && head) {
		prev->next = head->next;
		head->next = NULL;
		free(head);
		obj_db->count--;
	}
	return;
}

void xfree(object_db_t* object_db, void *ptr) {
	
	if (!ptr)	return;

	object_db_rec_t* obj_rec = object_db_lookup(object_db, ptr);
	assert(obj_rec);
	assert(obj_rec->ptr);

	delete_record_from_object_db(object_db, obj_rec);

	free(obj_rec->ptr);
	obj_rec->ptr = NULL;
	return;
}