#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>

#define MAX_DATA 200
#define MAX_ROW  110000


struct Address
{ 
  int id;
  int set;
  char name[MAX_DATA];
  char email[MAX_DATA];
};

struct Database
{
  struct Address rows[MAX_ROW];
};


struct Connection
{
  FILE *file;
  struct Database *db;
};

void die(const char *message){
  if(errno){
    perror(message);
  }
  else {
    printf("ERROR: %s\n",message);
  }
  exit(1);
}

void Address_print(struct Address *addr){
  printf("%d %s %s\n", addr->id, addr->name, addr->email);
}

void Database_load(struct Connection *conn){
  int rc = fread(conn -> db, sizeof(struct Database), 1, conn -> file);
  if (rc != 1) die("failed to load Database");
}

struct Connection *Database_open(const char *filename, char mode){
  struct Connection *conn = malloc(sizeof(struct Connection));
  if(conn == NULL) die("memory error");
  conn -> db = malloc(sizeof(struct Database));
  if(!conn -> db) die("memory error");
  if (mode == 'c'){
    conn -> file  = fopen(filename, "w");
  }
  else {
    conn -> file  = fopen(filename, "r+");
    if (conn -> file ){
      Database_load(conn);
    } 
  }
  if(!conn ->file){
    die("failed to open file");
  }
  return conn;
}

void Database_create(struct Connection *conn){
  int i = 0;
  for(i = 0; i< MAX_ROW; i++){
    struct Address v = {.id = i, .set = 0};
    conn ->db->rows[i] = v;
  }
}

void Database_write(struct Connection *conn){
  rewind(conn->file);
  int rc = fwrite(conn -> db, sizeof(struct Database), 1, conn -> file);
  if(rc  != 1) die("failed to write to Database");
  rc      = fflush(conn ->file);
  if(rc  != 0) die("failed to write to Database"); 
}

void Database_get(struct Connection *conn, int id){
  struct Address *addr = &conn->db->rows[id];
  if (addr == NULL) die("Couldnt get from database");
  if(addr->set){
    Address_print(addr);
  }
  else{
    die("id is not set");
  }
}

void Database_set(struct Connection *conn, int id, const char *name, const char *email){
  struct Address *addr = &(conn->db->rows[id]);
  if (addr->set) die("already set,please delete the row first!");
  addr->set = 1;
  strncpy(addr->name, name, MAX_DATA);
  strncpy(addr->email, email, MAX_DATA);
}

void Database_list(struct Connection *conn){
  int i = 0;
  struct Database *db = conn -> db;
  for(i = 0; i<MAX_ROW ; i++){
    struct Address *dat = &(db -> rows[i]);
    if(dat -> set){
      Address_print(dat);
    }
  }
}

void Database_close(struct Connection *conn){
  if(conn){
    if(conn->file) fclose(conn->file);
    if(conn->db) free(conn->db);
    free(conn);
  }
}

void Database_find(struct Connection *conn,const char *name){
  int i = 0;
  struct Address *addr = &(conn->db->rows);
  for (i = 0; i <= MAX_ROW; ++i){
    printf("Searching for record,looked %d rows\n",i+1);
    if(strcmp((addr+i)->name, name) == 0){
      printf("Found record!\n");
      Address_print(addr + i);
      return;
    }
  }
}

int main(int argc, char const *argv[])
{
  if(argc < 3) die("USAGE: sql <dbfile> <action> [action params]");

  char *filename          = argv[1];
  char action             = argv[2][0];
  struct Connection *conn = Database_open(filename, action);
  int id = 0;
  
  if(id >= MAX_ROW) die("there's not that many rows"); 
  switch(action){
    case 'c':
      Database_create(conn);
      Database_write(conn);
      break;
    case 'g':
      id = atoi(argv[3]);
      if(argc != 4) die("Need and id to get");
      Database_get(conn, id);
      break;
    case 's':
      id = atoi(argv[3]);
      if(argc !=6) die("need id, name, email to set");
      Database_set(conn, id, argv[4], argv[5]);
      Database_write(conn);
      break;
    case 'l':
      Database_list(conn);
      break;
    case 'f':
      Database_find(conn, argv[3]);
      break;
    default:
      die("Invalid action, only: c=create, g=get, s=set, d=del, l=list");

  }  
  Database_close(conn);
  return 0;
}