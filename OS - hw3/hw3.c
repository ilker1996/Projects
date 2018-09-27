#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "ext2.h"

#define BASE_OFFSET 1024
#define EXT2_BLOCK_SIZE 1024
#define lostfound 11

typedef unsigned char bmap;
#define __NBITS (8 * (int) sizeof (bmap))
#define __BMELT(d) ((d) / __NBITS)
#define __BMMASK(d) ((bmap) 1 << ((d) % __NBITS))
#define BM_SET(d, set) ((set[__BMELT (d)] |= __BMMASK (d)))
#define BM_CLR(d, set) ((set[__BMELT (d)] &= ~__BMMASK (d)))
#define BM_ISSET(d, set) ((set[__BMELT (d)] & __BMMASK (d)) != 0)

struct information
{	
	unsigned int inode_order;
	unsigned int deletion_time;
	unsigned int number_of_blocks;
	unsigned int inodenumber;
	unsigned int isRecovered;
};


bmap* block_bitmap;
bmap* inode_bitmap;

unsigned int block_size = 0;
unsigned int recovered_files = 0; // NUMBER OF ADDED FILES TO LOSTFOUND DIRECTORY
unsigned int deleted_files = 0; // NUMBER OF MARKED INODES AS DELETED
unsigned int total_length = 0; // JUMP POINT OF LOSTFOUND DATA BLOCK
unsigned int current_block = 0; // AVAILABLE DATA BLOCK OF LOSTFOUND DIRECTORY TO PUT ENTRY
struct information* deleted_array;

#define BLOCK_OFFSET(block) (BASE_OFFSET + (block-1)*block_size)


void read_block_of_inode(int fd,unsigned int inode_table,unsigned int inode_number);

int check_block_bitmap(unsigned int block_number,int fd);

int inode_isDeleted(int fd,unsigned int inode_table,unsigned int inode_number);

int isOverwritten(int fd,unsigned int inode_table,unsigned int inode_number);

void add_to_lostfound(int fd,unsigned int inodenumber,struct ext2_inode* lostfoundptr,unsigned int inode_order);

void handle_recovered_inode(int fd,unsigned inodetable,unsigned int inodenumber,struct ext2_inode* inode);

void direct_block(unsigned int block_number,int fd);

void indirect_block(unsigned int block_number,int fd);

void double_block(unsigned int block_number,int fd);

void triple_block(unsigned int block_number,int fd);

void copy_name(char dest[],unsigned int inode_order);


int main(int argc,char* args[])
{
    struct ext2_super_block super;
    struct ext2_group_desc group;
    struct ext2_inode lostfoundptr;
    int fd;
    int i,j = 0;
    char IMAGE[30]; //IMAGE NAME
    
    //COMMAND LINE EXCEPTION
    if(argc != 2){
        fprintf(stderr,"Write an image name");
        exit(1);
    }
    strcpy(IMAGE,args[1]);//COPY ARGUMENT STRING TO IMAGE VARIABLE

    // IF IT CAN NOT OPEN PROPERLY EXIT
    if ((fd = open(IMAGE, O_RDWR)) < 0) {
        perror(IMAGE);
        exit(1);
    }

    // read super-block
    lseek(fd, BASE_OFFSET, SEEK_SET);
    read(fd, &super, sizeof(super));
    if (super.s_magic != EXT2_SUPER_MAGIC) {
        fprintf(stderr, "Not a Ext2 filesystem\n");
        exit(1);
    }
    block_size = 1024 << super.s_log_block_size;// s_log_block_size == 0 (WHy?)

    // read group descriptor
    lseek(fd, BASE_OFFSET + block_size, SEEK_SET);
    read(fd, &group, sizeof(group));

    
    // READ BLOCK BITMAP
    block_bitmap = malloc(block_size);
 	lseek(fd,BLOCK_OFFSET(group.bg_block_bitmap),SEEK_SET);
 	read(fd,block_bitmap,block_size);

 	//READ INODE BITMAP
 	inode_bitmap = malloc(block_size);
 	lseek(fd,BLOCK_OFFSET(group.bg_inode_bitmap),SEEK_SET);
 	read(fd,inode_bitmap,block_size);

    
    unsigned int inode_count = super.s_inodes_per_group;    // NUMBER OF INODES PER GROUP
    
    struct ext2_dir_entry test;
    
    //READ LOSTFOUND INODE
    lseek(fd,BLOCK_OFFSET(group.bg_inode_table) + (lostfound-1)*sizeof(struct ext2_inode),SEEK_SET);
    read(fd,&lostfoundptr,sizeof(struct ext2_inode));
    
    lseek(fd,BLOCK_OFFSET(lostfoundptr.i_block[0]),SEEK_SET);	// DIRECTORY ENTRIES
    read(fd,&test,sizeof(struct ext2_dir_entry));	// READ FIRST DIRECTORY ENTRY(CURRENT DIRECTORY)
    total_length += test.rec_len;

    j = super.s_first_ino + 1;   // START FROM FIRST NON_RESERVED INODE
    
    				// COUNT DELETED INODES
    while(j <= inode_count){
    	if(inode_isDeleted(fd,group.bg_inode_table,j) == 1) // INODE IS DELETED
    	{	
    		deleted_files++;
        }
    	j++;
    }

 	deleted_array = (struct information*) malloc(sizeof(struct information) * deleted_files); // DELETED INODES

    struct ext2_inode temp_inode;
    int index = 0;
    j = 0;
    while(j <= inode_count){
       	if(inode_isDeleted(fd,group.bg_inode_table,j) == 1) // INODE IS DELETED
    	{	
    		lseek(fd,BLOCK_OFFSET(group.bg_inode_table) + (j-1)*sizeof(struct ext2_inode),SEEK_SET);
    		read(fd,&temp_inode,sizeof(struct ext2_inode));
    		deleted_array[index].deletion_time = temp_inode.i_dtime;
    		deleted_array[index].number_of_blocks = temp_inode.i_blocks / (block_size/512);
    		deleted_array[index].inodenumber = j;
    		deleted_array[index].inode_order = index + 1;
    		deleted_array[index].isRecovered = 0;

    		index++;
       	}
    	j++;
    }
    		

    		// SORT BY DELETION TIME
    struct information temp;
    int k,z;
    for (k = 0; k < deleted_files ;k++)
    {	
        for (z = 0; z < (deleted_files - k - 1); z++)
        {	
            if (deleted_array[z].deletion_time < deleted_array[z + 1].deletion_time)
            {
                temp = deleted_array[z];
                deleted_array[z] = deleted_array[z + 1];
                deleted_array[z + 1] = temp;
            }
        }
    }
    
    			     //TRAVERSE ALL INODES
    char* tempstring = malloc(sizeof(char) * 6);
    j = deleted_files; 
    deleted_files = 0;
    while(deleted_files < j){

    		// IF RECOVARABLE
   		if(isOverwritten(fd,group.bg_inode_table,deleted_array[deleted_files].inodenumber) == 0)
   		{	

   			deleted_array[deleted_files].isRecovered = 1;	// MARK AS RECOVERED
   			recovered_files++;
   	    	add_to_lostfound(fd,deleted_array[deleted_files].inodenumber,&lostfoundptr,deleted_array[deleted_files].inode_order);
       	}
    	deleted_files++;
    }
    
    deleted_files = j;
    		// SORT BY INODE ORDER
    for (k = 0; k < deleted_files ;k++)
    {	
        for (z = 0; z < (deleted_files - k - 1); z++)
        {	
            if (deleted_array[z].inode_order > deleted_array[z + 1].inode_order)
            {
                temp = deleted_array[z];
                deleted_array[z] = deleted_array[z + 1];
                deleted_array[z + 1] = temp;
            }
        }
    }
    //PRINT ALL DELETED FILES
    for(i = 0; i < deleted_files;i++)
   	{	
   		copy_name(tempstring,deleted_array[i].inode_order);
   		printf("%.6s %u %u\n",tempstring,deleted_array[i].deletion_time,deleted_array[i].number_of_blocks);
   	}
   	//IF THERE IS A DELETED FILE
    	printf("###\n");
    
    //PRINT RECOVERED FILES
    for(i = 0;i < deleted_files;i++)
    {
    	if(deleted_array[i].isRecovered == 1)
    	{
    		copy_name(tempstring,deleted_array[i].inode_order);
    		printf("%.6s\n",tempstring);
    	}    
    }
 	
    //ADJUST BLOCK BITMAP 
    lseek(fd,BLOCK_OFFSET(group.bg_block_bitmap),SEEK_SET);
    write(fd,block_bitmap,block_size);

    //ADJUST INODE BITMAP
    lseek(fd,BLOCK_OFFSET(group.bg_inode_bitmap),SEEK_SET);
    write(fd,inode_bitmap,block_size);

    free(deleted_array);
    deleted_array = NULL;
    free(tempstring);
    tempstring = NULL;
    free(block_bitmap);
    block_bitmap = NULL;
    free(inode_bitmap);
    inode_bitmap = NULL;
    close(fd);
    return 0;
}

int check_block_bitmap(unsigned int block_number,int fd){

    if(BM_ISSET(block_number,block_bitmap))
    	return 1;
    else
    	return 0;
    
}	

int isOverwritten(int fd,unsigned int inode_table,unsigned int inode_number)
{
	int i;
    struct ext2_inode inode;
	lseek(fd,BLOCK_OFFSET(inode_table)+(inode_number-1)*sizeof(struct ext2_inode),SEEK_SET);
	read(fd,&inode,sizeof(struct ext2_inode));
	for(i = 0; i < 15;i++)
	{
		if(inode.i_block[i] != 0 && check_block_bitmap(inode.i_block[i],fd) == 1)
		{
			return 1;	// BLOCK ARE USED BY ANOTHER INODE
		}
	}
	handle_recovered_inode(fd,inode_table,inode_number,&inode);//MARK AS REGULAR FOR RECOVERY
	return 0; // BLOCKS ARE AVAILABLE 
}

int inode_isDeleted(int fd,unsigned int inode_table,unsigned int inode_number)
{	
	struct ext2_inode inode;
	lseek(fd,BLOCK_OFFSET(inode_table)+(inode_number-1)*sizeof(struct ext2_inode),SEEK_SET);
	read(fd,&inode,sizeof(struct ext2_inode));
	if(inode.i_dtime > 0)	// IF DELETED  RETURN 1
	{
		return 1;
	}
	return 0;
}

void add_to_lostfound(int fd,unsigned int inodenumber,struct ext2_inode* lostfoundptr,unsigned int inode_order)
{   
	int temp;
	struct ext2_dir_entry* entry = malloc(sizeof(struct ext2_dir_entry));

   							 //HANDLE LAST ENTRY OF LOSTFOUND
   	lseek(fd,BLOCK_OFFSET(lostfoundptr -> i_block[current_block]) + total_length,SEEK_SET);
    read(fd,entry,sizeof(struct ext2_dir_entry));
    temp = entry -> name_len + (unsigned int) sizeof(struct ext2_dir_entry);
    if(temp % 4 != 0)
    {
    	temp = ((temp / 4) + 1) * 4;	// ACTUAL RECORD LENGTH OF LAST ENTRY
    }
    if(total_length + temp + 16 >= block_size) // IF BLOCK IS FULL
    {	
		total_length = 0;
    	current_block++;
    }
    else
    {
    	entry -> rec_len = temp;
    	lseek(fd,BLOCK_OFFSET(lostfoundptr -> i_block[current_block]) + total_length,SEEK_SET);
    	write(fd,(void*)entry,sizeof(struct ext2_dir_entry));
    	total_length += temp;
    }
    
    				//ADD RECOVERED FILE TO LOSTFOUND
	
	entry-> inode = inodenumber;
    entry-> file_type = EXT2_FT_REG_FILE;
    entry-> name_len = 6;
    entry-> rec_len = block_size - total_length; // FILL UP WHOLE BLOCK FOR CONVENTION
    lseek(fd,BLOCK_OFFSET(lostfoundptr -> i_block[current_block]) + total_length,SEEK_SET); // GO TO FIRST AVAILABLE ENTRY POINT
    write(fd,(void*)entry,sizeof(struct ext2_dir_entry));
	
	copy_name(entry->name,inode_order);
    write(fd,entry->name,entry->name_len*sizeof(char));			
	
	free(entry);
    entry = NULL;

}

void handle_recovered_inode(int fd,unsigned inodetable,unsigned int inodenumber,struct ext2_inode* inode)
{	
	int i;
	inode -> i_mode = EXT2_S_IFREG | EXT2_S_IRUSR;	// CHANGE INODE MODE
	inode -> i_dtime = 0;	// RESET DELETION TIME
	BM_SET(inodenumber,inode_bitmap); // SET CORRESPONDING INODE IN INODE BITMAP
	for(i = 0 ; i < 15 ; i++)
	{	
		//DIRECT BLOCK
		if(i < 12)
		{
			direct_block(inode -> i_block[i],fd);
		}
		//INDIRECT BLOCK
		if(i == 12)
		{
			indirect_block(inode -> i_block[i],fd);
		}
		//DIRECT BLOCK
		if(i == 13)
		{
			double_block(inode -> i_block[i],fd);
		}
		//TRIPLE BLOCK
		if(i == 14)
		{
			triple_block(inode -> i_block[i],fd);
		}
	}
	lseek(fd,BLOCK_OFFSET(inodetable) + (inodenumber - 1)*sizeof(struct ext2_inode),SEEK_SET);
	write(fd,inode,sizeof(struct ext2_inode));

}

void direct_block(unsigned int block_number,int fd)
{
	if(block_number != 0)
	{
		BM_SET(block_number,block_bitmap);
	}
}
void indirect_block(unsigned int block_number,int fd)
{	
	if(block_number != 0)
	{   
		int i;
		unsigned int size = block_size/sizeof(unsigned int);
		unsigned int array[size];
		lseek(fd,BLOCK_OFFSET(block_number),SEEK_SET);
		read(fd,&array,block_size);
		for(i= 0 ; i < size ; i++)
		{
			direct_block(array[i],fd);
		}

	}
}
void double_block(unsigned int block_number,int fd)
{
	if(block_number != 0)
	{   
		int i;
		unsigned int size = block_size/sizeof(unsigned int);
		unsigned int array[size];
		lseek(fd,BLOCK_OFFSET(block_number),SEEK_SET);
		read(fd,&array,block_size);
		for(i= 0 ; i < size ; i++)
		{
			indirect_block(array[i],fd);
		}

	}
}
void triple_block(unsigned int block_number,int fd)
{
	if(block_number != 0)
	{   
		int i;
		unsigned int size = block_size/sizeof(unsigned int);
		unsigned int array[size];
		lseek(fd,BLOCK_OFFSET(block_number),SEEK_SET);
		read(fd,&array,block_size);
		for(i= 0 ; i < size ; i++)
		{
			double_block(array[i],fd);
		}

	}
}


void copy_name(char dest[],unsigned int inode_order){
	if(inode_order < 10)
		sprintf(dest,"file0%u",inode_order);
	else
		sprintf(dest,"file%u",inode_order);
}