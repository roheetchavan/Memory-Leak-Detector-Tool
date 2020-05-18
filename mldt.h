
/*                                                                                                            
 * =====================================================================================                      
 *                                                                                                            
 *		  Filename:  mldt.h                                                                                     
 *                                                                                                            
 *		  Description:  This file defines the data structures used for MLD tool                                   
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


#ifndef __MLDT__
#include <assert.h>

#define MAX_STRUCT_NAME_SZ 120
#define MAX_FIELD_NAME_SZ 120

/*
 *	Data Type Enum
 */
typedef enum {
	UINT_8,
	UINT_32,
	INT_32,
	CHAR,
	OBJ_PTR,
	FLOAT,
	DOUBLE,
	OBJ_STRUCT
} data_type_t;


/*
 *	Useful macros
 */
#define OFFSETOF_FIELD(STRUCT_NAME, FIELD_NAME)		\
		((size_t)&(((STRUCT_NAME*)0)->FIELD_NAME))

#define SIZEOF_FIELD(STRUCT_NAME, FIELD_NAME)		\
		(sizeof(((STRUCT_NAME*)0)->FIELD_NAME))

/**********************************************************************************
 *																				  *
 *		Structure database defination starts									  *
 *																				  *
 *********************************************************************************/

/*
 *	Holds information of each feild of structure in c application
 */
typedef struct field_info {
	char name[MAX_FIELD_NAME_SZ];					/* name of field */
	data_type_t dtype;								/* data type of field */
	unsigned int size;								/* size of field */
	unsigned int offset;							/* offset of field */
	
	/* meaningful only if f_dtype is OBJ_PTR or OBJ_STRUCT	*/
	char nested_struct_name[MAX_STRUCT_NAME_SZ];
} field_info_t;

/*
 *	Holds information of c application structure which contains n fields
 */
typedef struct struct_db_rec {
	struct struct_db_rec* next;						/* pointer to next record in list */
	char name[MAX_STRUCT_NAME_SZ];					/* name of struct, used as key */
	unsigned int size;								/* size of struct */
	unsigned int nfields;							/* number of fields in struct */
	field_info_t* fields;							/* pointer to array of fields */
} struct_db_rec_t;

/*
 *	Head of the list representing structure database
 */
typedef struct struct_db {
	struct_db_rec_t* head;							/* head pointer */
	unsigned int count;								/* number of records in database */
} struct_db_t;

/*
 *  Structure Printing Functions
 */
void print_struct_record (struct_db_rec_t* struct_rec);
void print_struct_db (struct_db_t *struct_db);

/*
 *	Function to add record in staructure database
 *  Returns	 0 : success
 *			-1 : failure
 */
int add_struct_to_struct_db(struct_db_t *struct_db, struct_db_rec_t *struct_record);

/*
 *	Function to search for record in staructure database
 *  Returns	 record : success
 *			 NULL : failure
 */
struct_db_rec_t* struct_db_lookup(struct_db_t* struct_db, char *struct_name);

/*
 *	Structure Registration helping API's
 */
#define FIELD_INFO(struct_name, field_name, dtype, nested_struct_name)		\
	{	#field_name, dtype, SIZEOF_FIELD(struct_name, field_name),			\
		OFFSETOF_FIELD(struct_name, field_name), #nested_struct_name }

#define REGISTER_STRUCT(struct_db, struct_name, fields_arr)						\
	do{																			\
			struct_db_rec_t * record = calloc (1, sizeof(struct_db_rec_t));		\
			strncpy(record->name, #struct_name, MAX_STRUCT_NAME_SZ);			\
			record->size = sizeof(struct_name);									\
			record->nfields = sizeof(fields_arr)/sizeof(field_info_t);			\
			record->fields = fields_arr;										\
			if (add_struct_to_struct_db(struct_db, record)) {					\
				assert(0);														\
			}																	\
	}while(0);															

/**********************************************************************************
 *																				  *
 *		Structure database defination ends										  *
 *																				  *
 *********************************************************************************/

/**********************************************************************************
 *																				  *
 *		Object database defination starts										  *
 *																				  *
 *********************************************************************************/

/*
 *	Holds the information about object record 
 */
typedef struct object_db_rec {
	struct object_db_rec* next;					/* pointer to next reocrd in list */
	void *ptr;									/* pointer to memory location */
	unsigned int units;							/* number of memory chunks */
	struct_db_rec_t *struct_rec;				/* pointer to structure record */
} object_db_rec_t;

/*
 *	Head of the list
 */
typedef struct object_db {
	object_db_rec_t *head;						/* head pointer */
	struct_db_t *struct_db;						/* pinter to struct db */
	unsigned int count;							/* count of number of objects in list */	
} object_db_t;

/*
 *	Object printing functions
 */
void print_object_record (object_db_rec_t* object_rec);
void print_object_db (object_db_t *object_db);
void print_object_record_details(object_db_rec_t *obj_rec);
/**********************************************************************************
 *																				  *
 *		Object database defination ends											  *
 *																				  *
 *********************************************************************************/

/*
 *	Function to allocate memory
 */
void * xmalloc(object_db_t* object_db, char *struct_name, int units);
void xfree(object_db_t* object_db, void *ptr);

#endif /* __MLDT__*/
