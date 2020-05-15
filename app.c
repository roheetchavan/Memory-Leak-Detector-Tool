/*                                                                                                            
 * =====================================================================================                      
 *                                                                                                            
 *		  Filename:  app.c                                                                                     
 *                                                                                                            
 *		  Description: Basic application used to test MLD tool                                   
 *                                                                                                            
 *        This file is part of the Memery Leak Detector Tool distribution (https://github.com/roheetchavan).                          
 *        Copyright (c) 2020 Rohit Chavan. roheetchavan@gmail.com                                                                  
 *        This program is free software: you can redistribute it and/or modify                                
 *        it under the terms of the GNU General Public License as published by                                
 *        the Free Software Foundation, version 3.                                                            
 *                                                                                                            
 *        This program is distributed in the hope that it will be useful, but                                 
 *        WITHOUT ANY WARRANTY; without even the implied warranty of                                          
 *        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU                                    
 *        General Public License for more details.                                                            
 *                                                                                                           
 *        You should have received a copy of the GNU General Public License                                   
 *        along with this program. If not, see <http://www.gnu.org/licenses/>.                                
 *                                                                                                            
 * =====================================================================================                      
 */                                                            

#include "mldt.h"
#include <memory.h>
#include <stdlib.h>

/* Application Structure's */
typedef struct emp {
	char emp_name[30];
	unsigned int emp_id;
	unsigned int age;
	struct emp *mgr;
	double salary;
} emp_t;

typedef struct stud{
    char stud_name[32];
    unsigned int rollno;
    unsigned int age;
    float aggregate;
    struct stud *best_colleage;
} stud_t;


int main(int argc, char **argv) {

	/* Initialize structure database */
	struct_db_t *struct_db = calloc(1, sizeof(struct_db_t));
	

	/* Create structure record for emp */
	static field_info_t emp_fields[] = {
		FIELD_INFO(emp_t, emp_name, CHAR, 0),
		FIELD_INFO(emp_t, emp_id, UINT_32, 0),
		FIELD_INFO(emp_t, age, UINT_32, 0),
		FIELD_INFO(emp_t, mgr, OBJ_PTR, emp_t),
		FIELD_INFO(emp_t, salary, DOUBLE, 0)
	};
	REGISTER_STRUCT(struct_db, emp_t, emp_fields);
    
    /* Create structure record for student */
    static field_info_t stud_fields[] = {
		FIELD_INFO(stud_t, stud_name, CHAR, 0),
		FIELD_INFO(stud_t, rollno, UINT_32, 0),
		FIELD_INFO(stud_t, age, UINT_32, 0),
		FIELD_INFO(stud_t, aggregate, FLOAT, 0),
		FIELD_INFO(stud_t, best_colleage, OBJ_PTR, stud_t)
    };
    REGISTER_STRUCT(struct_db, stud_t, stud_fields);

	print_struct_db(struct_db);

	return 0;
}
