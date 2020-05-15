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
