/**
 * This program is licensed under the GNU General Public License,
 * version 2. A copy of the license can be found in the accompanying
 * LICENSE file.
 *
 **********************************************************************
 *
 * Simple program to limit the cpu usage of a process
 * If you modify this code, send me a copy please
 *
 * Author:  Angelo Marletta
 * Date:    26/06/2005
 * Version: 1.1
 *
 * Modifications and updates by: Jesse Smith
 * Date: May 4, 2011
 * Version 1.2 
 * Date: Jan 29, 2013
 * Version 1.2 and newer
 *
 * Modifications and updates by: Hasnain Lakhani
 * Date: Mar 26, 2014
 * Version 2.1
 */


#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/resource.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <limits.h>    // for compatibility


#include <dirent.h>
#define PROC_FILENAME 512
#define LINE_LENGTH 256

//kernel time resolution (inverse of one jiffy interval) in Hertz
//i don't know how to detect it, then define to the default (not very clean!)
#define HZ 100

//some useful macro
#define min(a,b) (a<b?a:b)
#define max(a,b) (a>b?a:b)

// For platforms without PATH_MAX
#ifndef PATH_MAX
#define PATH_MAX 4096
#endif
#ifndef EXEC_PATH
#define EXEC_PATH 32
#endif

#define BEST_PRIORITY -10

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#define VERSION 3.1

//pid of the controlled process
pid_t pid = 0;
pid_t my_pid;     // this process's PID

//executable file name
char *program_name;
//verbose mode
int verbose = FALSE;
//lazy mode
int lazy = FALSE;
// is higher priority nice possible?
int nice_lim;

// number of CPUs we detected
int NCPU;

// quiet mode
int quiet = FALSE;

// What signal should we send to the watched process
// when cpulimit exits?
int send_signal = SIGCONT;

//reverse byte search
// void *memrchr(const void *s, int c, size_t n);

#define MAX_SIGNAL 7
const char *SIGNAL_NAME[MAX_SIGNAL] = { "SIGHUP", "SIGINT", "SIGQUIT", 
                                 "SIGKILL", "SIGTERM", "SIGSTOP", "SIGCONT" };
const int SIGNAL_VALUE[MAX_SIGNAL] = { SIGHUP, SIGINT, SIGQUIT,
                                  SIGKILL, SIGTERM, SIGSTOP, SIGCONT };


//return ta-tb in microseconds (no overflow checks!)
long timediff(const struct timespec *ta,const struct timespec *tb) {
    unsigned long us = (ta->tv_sec-tb->tv_sec)*1000000 + (ta->tv_nsec/1000 - tb->tv_nsec/1000);
    return us;
}

#include <pthread.h>

typedef struct
{
   pid_t child;   // the child of our target process
   pid_t monitor; // the LimitCPU fork monitoring the child
   void *next; 
} CHILD;

int quitting = FALSE;           // Have we receive a quit signal
int monitor_children = FALSE;   // are we monitoring children of target

// Data passed to the monitor thread
typedef struct
{
   int limit;           // per cent limit to place on a process
   char *this_program;  // copy of argv[0]
} PROGRAM_DATA;


int Check_Us(pid_t target_pid)
{
   pid_t this_pid;

   this_pid = getpid();
   if (this_pid == target_pid)
   {
      fprintf(stderr, "We cannot throttle ourselves.\n");
      exit(7);
   }
   return TRUE;
}


int waitforpid(int pid) {
  //switch to low priority
  // if (setpriority(PRIO_PROCESS,getpid(),19)!=0) {
        /*
        if ( (nice_lim < INT_MAX) && 
             (setpriority(PRIO_PROCESS, my_pid, 19) != 0) ) {
    printf("Warning: cannot renice\n");
  }
        */
  int i=0;

  while(1) {

    DIR *dip;
    struct dirent *dit;

    //open a directory stream to /proc directory
    if ((dip = opendir("/proc")) == NULL) {
      perror("opendir");
      return -1;
    }

    //read in from /proc and seek for process dirs
    while ((dit = readdir(dip)) != NULL) {
      //get pid
      if (pid==atoi(dit->d_name)) {
        //pid detected
                                Check_Us(pid);
        if (kill(pid,SIGSTOP)==0 &&  kill(pid,SIGCONT)==0) {
          //process is ok!
                                        if (closedir(dip) == -1) {
                                           perror("closedir");
                                           return -1;
                                        }
          goto done;
        }
        else {
          fprintf(stderr,"Error: Process %d detected, but you don't have permission to control it\n",pid);
        }
      }
    }

    //close the dir stream and check for errors
    if (closedir(dip) == -1) {
      perror("closedir");
      return -1;
    }

    //no suitable target found
    if (i++==0) {
      if (lazy) {
        fprintf(stderr,"No process found\n");
        exit(2);
      }
      else {
        fprintf(stderr, "Warning: no target process found. Waiting for it...\n");
      }
    }

    //sleep for a while
    sleep(2);
  }

done:
    if (!quiet)
      printf("Process %d detected\n",pid);

    //now set high priority, if possible
    // if (setpriority(PRIO_PROCESS,getpid(),-20)!=0) {
    /*
    if ( (nice_lim < INT_MAX) &&
         (setpriority(PRIO_PROCESS, my_pid, nice_lim) != 0) ) {
       printf("Warning: cannot renice.\nTo work better you should run this program as root.\n");
    }
    */
    return 0;

}

//this function periodically scans process list and looks for executable path names
//it should be executed in a low priority context, since precise timing does not matter
//if a process is found then its pid is returned
//process: the name of the wanted process, can be an absolute path name to the executable file
//         or simply its name
//return: pid of the found process
int getpidof(const char *process) {

  //set low priority
  // if (setpriority(PRIO_PROCESS,getpid(),19)!=0) {
        /*
        if ( (nice_lim < INT_MAX) &&
             (setpriority(PRIO_PROCESS, my_pid, 19) != 0) ) {
    printf("Warning: cannot renice\n");
  }
        */
  char exelink[EXEC_PATH + 1];
  char exepath[PATH_MAX+1];
  int pid=0;
  int i=0;

  while(1) {

    DIR *dip;
    struct dirent *dit;

    //open a directory stream to /proc directory
    if ((dip = opendir("/proc")) == NULL) {
      perror("opendir");
      return -1;
    }

    //read in from /proc and seek for process dirs
    while ((dit = readdir(dip)) != NULL) {
      //get pid
      pid=atoi(dit->d_name);
      if (pid>0) {
        snprintf(exelink, EXEC_PATH, "/proc/%d/exe",pid);
        int size=readlink(exelink,exepath,sizeof(exepath));
        if (size>0) {
          int found=0;
          if (process[0]=='/' && strncmp(exepath,process,size)==0 && size==strlen(process)) {
            //process starts with / then it's an absolute path
            found=1;
          }
          else {
            //process is the name of the executable file
            if (strncmp(exepath+size-strlen(process),process,strlen(process))==0) {
              found=1;
            }
          }
          if (found==1) {
                                        Check_Us(pid);
            if (kill(pid,SIGSTOP)==0 &&  kill(pid,SIGCONT)==0) {
              //process is ok!
                                                        if (closedir(dip) == -1) {
                                                          perror("closedir");
                                                          return -1;
                                                        }
              goto done;
            }
            else {
              fprintf(stderr,"Error: Process %d detected, but you don't have permission to control it\n",pid);
            }
          }
        }
      }
    }

    //close the dir stream and check for errors
    if (closedir(dip) == -1) {
      perror("closedir");
      return -1;
    }

    //no suitable target found
    if (i++==0) {
      if (lazy) {
        fprintf(stderr,"No process found\n");
        exit(2);
      }
      else {
        fprintf(stderr, "Warning: no target process found. Waiting for it...\n");
      }
    }

    //sleep for a while
    sleep(2);
  }

done:
    if (!quiet)
      printf("Process %d detected\n",pid);

    //now set high priority, if possible
    // if (setpriority(PRIO_PROCESS,getpid(),-20)!=0) {
    /*
    if ( (nice_lim < INT_MAX) &&
         (setpriority(PRIO_PROCESS, my_pid, nice_lim) != 0) ) {
       printf("Warning: cannot renice.\nTo work better you should run this program as root.\n");
    }
    */
    return pid;
}

//SIGINT and SIGTERM signal handler
void quit(int sig) {
  //let the process continue if we are stopped
  kill(pid, send_signal);
        #ifdef LINUX
        if (monitor_children)
        {
            quitting = TRUE;
            printf("Asking children to quit...\n");
            sleep(2);                // wait for thread clean-up
        }
        #endif
  printf("Exiting...\n");
  exit(0);
}

// Handle a child process quitting
void Child_Done(int sig)
{
   pid_t caught_child;
   caught_child = waitpid(-1, NULL, WNOHANG);
   if (verbose && caught_child != 0) {
      printf("Caught child process: %ld\n", (long int) caught_child);
      printf("%d\n", errno);
   }
   // If this was the one process we were watching, we can quit now.
   if (caught_child == pid) {
      if (verbose) printf("Child process is finished, exiting...\n");
      exit(0);
   }
}


//get jiffies count from /proc filesystem
long getjiffies(int pid) {
  static char stat[20];
  static char buffer[1024];
        char *p;
  sprintf(stat,"/proc/%d/stat",pid);
  FILE *f=fopen(stat,"r");
  if (f==NULL) return -1;
  p = fgets(buffer,sizeof(buffer),f);
  fclose(f);
  // char *p=buffer;
        if (p)
        {
    p=memchr(p+1,')',sizeof(buffer)-(p-buffer));
    int sp=12;
    while (sp--)
    p=memchr(p+1,' ',sizeof(buffer)-(p-buffer));
    //user mode jiffies
    long utime=atol(p+1);
    p=memchr(p+1,' ',sizeof(buffer)-(p-buffer));
    //kernel mode jiffies
    long ktime=atol(p+1);
    return utime+ktime;
        }
        // could not read info
        return -1;
}


//process instant photo
struct process_screenshot {
  struct timespec when;  //timestamp
  long jiffies;  //jiffies count of the process
  int cputime;  //microseconds of work from previous screenshot to current
};

//extracted process statistics
struct cpu_usage {
  float pcpu;
  float workingrate;
};

//this function is an autonomous dynamic system
//it works with static variables (state variables of the system), that keep memory of recent past
//its aim is to estimate the cpu usage of the process
//to work properly it should be called in a fixed periodic way
//perhaps i will put it in a separate thread...
int compute_cpu_usage(int pid,int last_working_quantum,struct cpu_usage *pusage) {
  #define MEM_ORDER 10
  //circular buffer containing last MEM_ORDER process screenshots
  static struct process_screenshot ps[MEM_ORDER];
  //the last screenshot recorded in the buffer
  static int front=-1;
  //the oldest screenshot recorded in the buffer
  static int tail=0;

  if (pusage==NULL) {
    //reinit static variables
    front=-1;
    tail=0;
    return 0;
  }

  //let's advance front index and save the screenshot
  front=(front+1)%MEM_ORDER;
  long j=getjiffies(pid);
  if (j>=0) ps[front].jiffies=j;
  else return -1;  //error: pid does not exist

        // Linux and BSD can use real time
  clock_gettime(CLOCK_REALTIME,&(ps[front].when));
  ps[front].cputime=last_working_quantum;
  //buffer actual size is: (front-tail+MEM_ORDER)%MEM_ORDER+1
  int size=(front-tail+MEM_ORDER)%MEM_ORDER+1;

  if (size==1) {
    //not enough samples taken (it's the first one!), return -1
    pusage->pcpu=-1;
    pusage->workingrate=1;
    return 0;
  }
  else {
    //now we can calculate cpu usage, interval dt and dtwork are expressed in microseconds
    long dt=timediff(&(ps[front].when),&(ps[tail].when));
    long dtwork=0;
    int i=(tail+1)%MEM_ORDER;
    int max=(front+1)%MEM_ORDER;
    do {
      dtwork+=ps[i].cputime;
      i=(i+1)%MEM_ORDER;
    } while (i!=max);
    long used=ps[front].jiffies-ps[tail].jiffies;
    float usage=(used*1000000.0/HZ)/dtwork;
    pusage->workingrate=1.0*dtwork/dt;
    pusage->pcpu=usage*pusage->workingrate;
    if (size==MEM_ORDER)
      tail=(tail+1)%MEM_ORDER;
    return 0;
  }
  #undef MEM_ORDER
}

void print_caption() {
  printf("\n%%CPU\twork quantum\tsleep quantum\tactive rate\n");
}


void increase_priority()
{
        //find the best available nice value
        int old_priority = getpriority(PRIO_PROCESS, 0);
        int priority = old_priority;
        while ( (setpriority(PRIO_PROCESS, 0, priority-1) == 0) &&
                (priority > BEST_PRIORITY) )
        {
                priority--;
        }
        if (priority != old_priority) {
                if (verbose) printf("Priority changed to %d\n", priority);
        }
        else {
                if (verbose) printf("Warning: Cannot change priority. Run as root or renice for best results.\n");
        }


}


// This following functions are for detecting and limiting child
// processes on Linux.

// This function adds a new child process to our list of child processes.
CHILD *Add_Child(CHILD *all_children, pid_t new_pid)
{
    CHILD *new_child = (CHILD *) calloc(sizeof(CHILD), 1);
    CHILD *current;

    if (! new_child)
       return all_children;

    new_child->next = NULL;
    new_child->child = new_pid;
    new_child->monitor = 0;

    if (all_children)
    {
        current = all_children;
        while (current->next)
           current = current->next;
        current->next = new_child;
        return all_children;
    }
    else   // this is the first node
       return new_child;
}



// This function removes a child PID node.
CHILD *Remove_Child(CHILD *all_children, pid_t old_pid)
{
    CHILD *current, *previous = NULL;
    int found = FALSE;

    current = all_children;
    while ( (! found) && (current) )
    {
        if (current->child == old_pid)
        {
           current->monitor = 0;
           current->child = 0;
           if (previous)
               previous->next = current->next;
           else
             all_children = current->next;
           
           free(current);
           found = TRUE;
        }
        else
        {
            previous = current;
            current = current->next;
        }
     }
     return all_children;
}


// This function cleans up all remaining child nodes.
void Clean_Up_Children(CHILD *all_children)
{
   CHILD *current, *next;

   current = all_children;
   while (current)
   {
      next = current->next;
      free(current);
      current = next;
   }
}


// This function searches the linked list for a matching PID.
// It returns NULL if no match is found and a pointer to the
// node is a match is located.
CHILD *Find_Child(CHILD *children, pid_t target)
{
   CHILD *current;
   int found = FALSE;

   current = children;
   while ( (!found) && (current) )
   {
      if (current->child == target)
        found = TRUE;
      else
        current = current->next;
   }
   return current;
}



// This function returns a list of process IDs of children
// of the given process (PID). It does this by searching the /proc
// file system and looking in the /proc/pid/status file for the PPid field.
// A linked list of child PIDs is returned on success or NULL on failure or
// if no child PIDs are found.
CHILD *Find_Child_PIDs(CHILD *all_children, pid_t parent_pid)
{
    int found = FALSE;
    DIR *proc;
    struct dirent *proc_entry;
    char filename[PROC_FILENAME + 1];
    FILE *status_file;
    char *reading_file;
    char line[256];
    pid_t new_ppid;
    int current_pid;
    CHILD *my_children = NULL;

    proc = opendir("/proc");
    if (! proc)
       return NULL;

    proc_entry = readdir(proc);
    while (proc_entry)
    {
        snprintf(filename, PROC_FILENAME, "/proc/%s/status", proc_entry->d_name);
        status_file = fopen(filename, "r");
        if (status_file)
        {
           found = FALSE;
           reading_file = fgets(line, LINE_LENGTH, status_file);
           while ( (! found) && (reading_file) )
           {
             if (! strncmp(line, "PPid:", 5) )
             {
                 sscanf(&(line[6]), "%d", &new_ppid);
                 if (new_ppid == parent_pid && current_pid != getpid() )
                 {
                     sscanf(proc_entry->d_name, "%d", &current_pid);
                     if (! Find_Child(all_children, current_pid) )
                        my_children = Add_Child(all_children, current_pid);
                 }
                 found = TRUE;
             }
             else
                reading_file = fgets(line, LINE_LENGTH, status_file);
           }   // done reading status file
          
           fclose(status_file);
        }
        proc_entry = readdir(proc);
    }   // done reading proc file system
    closedir(proc);
    return my_children;
}


// This function (which should probably be called as a thread) monitors the
// system for child processes of the current target process. When a new
// child of the target is located, it is added to the CHILD list.
// New children result in a new fork of this program being spawned to
// monitor the child process and its children.

void *Monitor_Children(void *all_data)
{
   CHILD *all_children = NULL;
   CHILD *current;
   PROGRAM_DATA *program_data = (PROGRAM_DATA *) all_data;

   while (! quitting )
   {
      // Check for new child processes
      all_children = Find_Child_PIDs(all_children, pid);

      // Find any children without monitors and create a monitoring process
      // Clean out old processes while we are looking
      current = all_children;
      while (current)
      {
         // First see if the child process is still running. If not,
         // we can remove its node.
         if (current->child)
         {
             char filename[PROC_FILENAME + 1];
             DIR *child_directory;
             snprintf(filename, PROC_FILENAME, "/proc/%ld", (long int) current->child);
             child_directory = opendir(filename);
             if (child_directory)
                closedir(child_directory);
             else
             {
                if (verbose)
                  printf("Child process %ld done, cleaning up.\n",
                          (long int) current->child);
                all_children = Remove_Child(all_children, current->child);
             }
         }  // end of clean up children processes no longer running

         // The child process is still running, but it might not have
         // a monitor. Create a new monitoring process.
         else if ( (current->child) && (! current->monitor) ) {
              pid_t returned_pid;

              if (verbose) printf("Creating monitoring process for %ld\n", (long int) current->child);

              returned_pid = fork();
              if (returned_pid > 0) {
                 // parent
                 current->monitor = returned_pid;

              } else if (returned_pid == 0) { // child
                 char limit_amount[16];
                 char process_identifier[16];
                 snprintf(limit_amount, 16, "%d", (int) program_data->limit);
                 snprintf(process_identifier, 16, "%ld", (long int) current->child);
                 if (verbose)
                    printf("Starting monitor with: %s -l %s -p %s -z -m\n",
                            program_data->this_program, limit_amount,
                            process_identifier);
                 execlp(program_data->this_program, program_data->this_program,
                       "-l", limit_amount, "-p", process_identifier, 
                       "-z", "-m", (char *) NULL);
                 // we should not return, report error and bail out
                 if (verbose) printf("Error trying to execute %s\n", program_data->this_program);
                 exit(1);
                  
              }
              
         }    // end of creating a new monitor
         if ( (verbose) && (! quitting) )
         {
             printf("Watching child: %ld with %ld\n", 
                   (long int) current->child, (long int) current->monitor);
         }
         current = current->next;
      }
      sleep(1);
   }  // end LimitCPU is still running
   pthread_exit(NULL);
}


void print_usage(FILE *stream,int exit_code) {
  fprintf(stream, "CPUlimit version %1.1f\n", VERSION);
  fprintf(stream, "Usage: %s TARGET [OPTIONS...] [-- PROGRAM]\n",program_name);
  fprintf(stream, "   TARGET must be exactly one of these:\n");
  fprintf(stream, "      -p, --pid=N        pid of the process\n");
  fprintf(stream, "      -e, --exe=FILE     name of the executable program file\n");
  fprintf(stream, "                         The -e option only works when\n");
  fprintf(stream, "                         cpulimit is run with admin rights.\n");
  fprintf(stream, "      -P, --path=PATH    absolute path name of the\n");
  fprintf(stream, "                         executable program file\n");
  fprintf(stream, "   OPTIONS\n");
  fprintf(stream, "      -c  --cpu=N        override the detection of CPUs on the machine.\n");
  fprintf(stream, "      -l, --limit=N      percentage of cpu allowed from 1 up.\n");
  fprintf(stream, "                         Usually 1 - %d00, but can be higher\n", NCPU);
  fprintf(stream, "                         on multi-core CPUs (mandatory)\n");
  fprintf(stream, "      -m, --monitor-forks  Watch children/forks of the target process\n");
  fprintf(stream, "      -q, --quiet        run in quiet mode (only print errors).\n");
  fprintf(stream, "      -k, --kill         kill processes going over their limit\n");
  fprintf(stream, "                         instead of just throttling them.\n");
  fprintf(stream, "      -r, --restore      Restore processes after they have\n");
  fprintf(stream, "                         been killed. Works with the -k flag.\n");
  fprintf(stream, "      -s, --signal=SIG   Send this signal to the watched process when cpulimit exits.\n");
  fprintf(stream, "                         Signal should be specificed as a number or \n");
  fprintf(stream, "                         SIGTERM, SIGCONT, SIGSTOP, etc. SIGCONT is the default.\n");
  fprintf(stream, "      -v, --verbose      show control statistics\n");
  fprintf(stream, "      -z, --lazy         exit if there is no suitable target process,\n");
  fprintf(stream, "                         or if it dies\n");
  fprintf(stream, "          --             This is the final CPUlimit option. All following\n");
  fprintf(stream, "                         options are for another program we will launch.\n");
  fprintf(stream, "      -h, --help         display this help and exit\n");
  exit(exit_code);
}



// Get the number of CPU cores on this machine.
int get_ncpu()
{
        int ncpu = 1;
#ifdef _SC_NPROCESSORS_ONLN
        ncpu = sysconf(_SC_NPROCESSORS_ONLN);
#endif
        return ncpu;
}


// This function attempts to figure out what signal we should send
// target processes based on a command line paramter. First we check
// for text such as SIGINT, SIGCONT, SIGSTOP, etc. If no match is found
// then we assume the value given is a number and use that.
int Translate_Signal(char *my_signal)
{
    int signal_value;
    int index = 0, found = FALSE;
    // first check to see if we were passed a string
    while ( (index < MAX_SIGNAL) && (! found) )
    {
        if (! strcmp(my_signal, SIGNAL_NAME[index]) )
        {
            found = TRUE;
            signal_value = SIGNAL_VALUE[index];
        }
        else
           index++;
    }

    // no value found, try a number
    if (! found)
       signal_value = atoi(my_signal);

    return signal_value;
}



int main(int argc, char **argv) {

  //get program name
  // char *p=(char*)memrchr(argv[0],(unsigned int)'/',strlen(argv[0]));
  // program_name = p==NULL?argv[0]:(p+1);
  program_name = argv[0];
  int run_in_background = FALSE;
  //parse arguments
  int next_option;
  /* A string listing valid short options letters. */
  const char* short_options="p:e:P:l:c:s:bfqkmrvzh";
  PROGRAM_DATA program_data;

  /* An array describing valid long options. */
  const struct option long_options[] = {
    { "pid", required_argument, NULL, 'p' },
    { "exe", required_argument, NULL, 'e' },
    { "path", required_argument, NULL, 'P' },
    { "limit", required_argument, NULL, 'l' },
    { "background", no_argument, NULL, 'b' },
    { "foreground", no_argument, NULL, 'f' },
    { "quiet", no_argument, NULL, 'q' },
    { "verbose", no_argument, NULL, 'v' },
    { "lazy", no_argument, NULL, 'z' },
    { "help", no_argument, NULL, 'h' },
    { "cpu", required_argument, NULL, 'c'},
    { "signal", required_argument, NULL, 's'},
    { "monitor-forks", no_argument, NULL, 'm'},
    { NULL, 0, NULL, 0 }
  };
  //argument variables
  const char *exe=NULL;
  const char *path=NULL;
  int perclimit=0;
  int pid_ok = FALSE;
  int process_ok = FALSE;
  int limit_ok = FALSE;
  int last_known_argument = 0;
  int kill_process = FALSE;   // kill process instead of stopping it
  int restore_process = FALSE;  // restore killed process
  int run_child_in_background = TRUE;  // run cpulimit in background when 
                                             //  we launch new process
  // struct rlimit maxlimit;

  NCPU = get_ncpu();

  opterr = 0;      // avoid unwanted error messages for unknown parameters
  do {
    next_option = getopt_long (argc, argv, short_options,long_options, NULL);
    switch(next_option) {
      case 'b':
        run_in_background = TRUE;
        last_known_argument++;
        break;
      case 'f':
        run_child_in_background = FALSE;
        run_in_background = FALSE;
        last_known_argument++;
        break;
      case 'p':
        pid=atoi(optarg);
        if (pid) {  // valid PID
          pid_ok = TRUE;
          lazy = TRUE;
        }
        last_known_argument += 2;
        break;
      case 'e':
        exe=optarg;
        process_ok = TRUE;
        last_known_argument += 2;
        break;
      case 'P':
        path=optarg;
        process_ok = TRUE;
        last_known_argument += 2;
        break;
      case 'l':
        perclimit=atoi(optarg);
        limit_ok = TRUE;
        last_known_argument += 2;
        break;
      case 'c':
        NCPU = atoi(optarg);
        last_known_argument += 2;
        break;
      case 's':
        send_signal = Translate_Signal(optarg);
        if ( (send_signal < 1) || (send_signal > 35) ) {
          fprintf(stderr, "Specified exit signal is not recognized or not within bounds (1-35). Using SIGCONT.\n");
          send_signal = SIGCONT;
        }
        last_known_argument += 2;
      case 'k':
        kill_process = TRUE;
        last_known_argument++;
        break;
      case 'm':
        monitor_children = TRUE;
        last_known_argument++;
        break;
      case 'r':
        restore_process = TRUE;
        last_known_argument++;
        break;
      case 'v':
        verbose = TRUE;
        last_known_argument++;
        break;
      case 'q':
        quiet = TRUE;
        last_known_argument++;
        break;
      case 'z':
        lazy = TRUE;
        last_known_argument++;
        break;
      case 'h':
        print_usage (stdout, 1);
        last_known_argument++;
        break;
      case 'o':
        last_known_argument++;
        next_option = -1;
        break;
      case '?':
        print_usage (stderr, 1);
        last_known_argument++;
        break;
      case -1:
        break;
      // default:
      //  abort();
    }
  } while(next_option != -1);

  signal(SIGCHLD, Child_Done);

  // try to launch a program passed on the command line
  // But only if we do not already have a PID to watch
  if ( (last_known_argument + 1 < argc) && (pid_ok == FALSE) ) {
    last_known_argument++;
    // if we stopped on "--" jump to the next parameter
    if ( (last_known_argument + 1 < argc) && (! strcmp(argv[last_known_argument], "--") ) )
        last_known_argument++;
    pid_t forked_pid;

    // try to launch remaining arguments
    if (verbose) {
        int index = last_known_argument;
        printf("Launching %s", argv[index]);
        for (index = last_known_argument + 1; index < argc; index++)
             printf(" %s", argv[index]);
        printf(" with limit %d\n", perclimit);
    }

    forked_pid = fork();
    if (forked_pid == -1)  // error
    {
        printf("Failed to launch specified process.\n");
        exit(1);
    }
    else if (forked_pid == 0)   // target child
    {
       execvp(argv[last_known_argument],
              &(argv[last_known_argument]) );
       exit(2);
    }
    else     // parent who will now fork the throttler
    {
       pid_t limit_pid;
       // if we are planning to kill a process, give it
       // a running head start to avoid death at start-up
       if (kill_process) sleep(5);

       /* The following block assumes we want to run cpulimit in the
          background. This is the default behaviour.  */ 
       if (run_child_in_background) {
          limit_pid = fork();
          if (limit_pid == 0)   // child cpulimit process running in background
          {
             pid = forked_pid;    // the first child, target process
             lazy = TRUE;
             pid_ok = TRUE;
             if (verbose) printf("Throttling process %ld\n", (long int) pid);
          }
          else    // parent cpulimit process which can quit
            exit(0);
       }  // end of running in background
       else
       {
           pid = forked_pid;
           lazy = TRUE;
           pid_ok = TRUE;
           run_in_background = FALSE;
       }  // end of running in foreground

    }  // end of parent that launched target

  }      // end of launching child process

  if (!process_ok && !pid_ok) {
    fprintf(stderr,"Error: You must specify a target process\n");
    print_usage (stderr, 1);
    exit(1);
  }
  if ((exe!=NULL && path!=NULL) || (pid_ok && (exe!=NULL || path!=NULL))) {
    fprintf(stderr,"Error: You must specify exactly one target process\n");
    print_usage (stderr, 1);
    exit(1);
  }
  if (!limit_ok) {
    fprintf(stderr,"Error: You must specify a cpu limit\n");
    print_usage (stderr, 1);
    exit(1);
  }
  float limit=perclimit/100.0;
  if ( (limit <= 0.00) || (limit > NCPU) )
        {
    fprintf(stderr,"Error: limit must be in the range of 1 to %d00\n", NCPU);
    print_usage (stderr, 1);
    exit(1);
  }

        // check to see if we should fork
        if (run_in_background)
        {
             pid_t process_id;
             process_id = fork();
             if (! process_id)
                exit(0);
             else
             {
                setsid();
                process_id = fork();
                if (process_id)
                  exit(0);
             }
        }

  //parameters are all ok!
  signal(SIGINT,quit);
  signal(SIGTERM,quit);

  my_pid = getpid();
  if (verbose) printf("%d CPUs detected.\n", NCPU);

  increase_priority();

  //time quantum in microseconds. it's splitted in a working period and a sleeping one
  int period=100000;
  struct timespec twork,tsleep;   //working and sleeping intervals
  memset(&twork,0,sizeof(struct timespec));
  memset(&tsleep,0,sizeof(struct timespec));

wait_for_process:

  //look for the target process..or wait for it
  if (exe != NULL) pid=getpidof(exe);
  else if (path != NULL) pid=getpidof(path);
  else waitforpid(pid);
  
  //process detected...let's play

  //init compute_cpu_usage internal stuff
  compute_cpu_usage(0,0,NULL);
  //main loop counter
  int i=0;

  struct timespec startwork,endwork;
  long workingtime=0;    //last working time in microseconds

  if (verbose) print_caption();

  float pcpu_avg=0;

  // On Linux we can monitor child processes of the target
  if (monitor_children) {
    pthread_t my_thread;
    int thread_status;
    if (verbose) printf("Starting fork monitoring thread...\n");
    program_data.this_program = argv[0];
    program_data.limit = perclimit;
    thread_status = pthread_create(&my_thread, NULL, Monitor_Children, &program_data);
    if ( (thread_status) && (verbose) ) printf("Creating fork monitoring thread failed.\n");
  }

  while(1) {

    //estimate how much the controlled process is using the cpu in its working interval
    struct cpu_usage cu;
    if (compute_cpu_usage(pid,workingtime,&cu)==-1) {
      if (!quiet) fprintf(stderr,"Process %d dead!\n",pid);
      if (lazy) exit(2);
      //wait until our process appears
      goto wait_for_process;    
    }

    //cpu actual usage of process (range 0-1)
    float pcpu=cu.pcpu;
    //rate at which we are keeping active the process (range 0-1)
    float workingrate=cu.workingrate;

    //adjust work and sleep time slices
    if (pcpu>0) {
      twork.tv_nsec=min(period*limit*1000/pcpu*workingrate,period*1000);
    } else if (pcpu==0) {
      twork.tv_nsec=period*1000;
    } else if (pcpu==-1) {
      //not yet a valid idea of cpu usage
      pcpu=limit;
      workingrate=limit;
      twork.tv_nsec=min(period*limit*1000,period*1000);
    }
    tsleep.tv_nsec=period*1000-twork.tv_nsec;

    //update average usage
    pcpu_avg=(pcpu_avg*i+pcpu)/(i+1);

    if (verbose && i%10==0 && i>0) {
      printf("%0.2f%%\t%6ld us\t%6ld us\t%0.2f%%\n",pcpu*100,twork.tv_nsec/1000,tsleep.tv_nsec/1000,workingrate*100);
      if (i%200 == 0) print_caption();
    }

    if (pcpu < limit) {
      //if (verbose) printf("Continue\n");
      //resume process
      if (kill(pid,SIGCONT)!=0) {
        if (!quiet) fprintf(stderr,"Process %d dead!\n",pid);
        if (lazy) exit(2);
        //wait until our process appears
        goto wait_for_process;
      }
    }

    clock_gettime(CLOCK_REALTIME,&startwork);

    nanosleep(&twork,NULL);    //now process is working
    clock_gettime(CLOCK_REALTIME,&endwork);
    workingtime = timediff(&endwork,&startwork);

    // printf("Checking %f vs %f\n", pcpu, limit);
    if (pcpu > limit) {
      // When over our limit we may run into
      // situations where we want to kill
      // the offending process, then restart it
      if (kill_process) {
        kill(pid, SIGKILL);
        if (!quiet)
            fprintf(stderr, "Process %d killed.\n", pid);
        if ( (lazy) && (! restore_process) ) 
             exit(2);
        // restart killed process
        if (restore_process) {
            pid_t new_process;
            new_process = fork();
            if (new_process == -1) {
             fprintf(stderr, "Failed to restore killed process.\n");

            } else if (new_process == 0) {
               // child which becomes new process
               if (verbose) printf("Relaunching %s\n", argv[last_known_argument]);
               execvp(argv[last_known_argument], &(argv[last_known_argument]) ); 
            } else { // parent
               // we need to track new process
               pid = new_process;
               // avoid killing child process
               sleep(5);
            }
        }

      } else { // do not kill process, just throttle it
        // printf("Stop\n");
        //stop process, it has worked enough
        if (kill(pid,SIGSTOP)!=0) {
          if (!quiet) fprintf(stderr,"Process %d dead!\n", pid);
          if (lazy) exit(2);
          //wait until our process appears
          goto wait_for_process;
        }
        nanosleep(&tsleep,NULL);  //now process is sleeping
      }   // end of throttle process
    }         // end of process using too much CPU
    i++;
  } // while

  return 0;
}
