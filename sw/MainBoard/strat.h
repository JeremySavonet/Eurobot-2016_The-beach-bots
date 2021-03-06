/*
 * Start module: Here is the fsm of the robot
 * Author: Jeremy S.
 * Rev: 1.0
 */

#ifndef _STRAT_H_
#define _STRAT_H_

// Time of a game in seconds
#define GAME_RUNNING_TIME 10

// Return values for trajectories => create an enum
#define END_TRAJ       1 // Trajectory successful
#define END_BLOCKING   2 // Blocking during trajectory
#define END_NEAR       4 // Arrived near point
#define END_OBSTACLE   8 // There is an obstacle in front of us
#define END_ERROR     16 // Cannot do the command
#define END_TIMER     32 // End of match timer.

// Checks if an return code indicates a succesful trajectory
#define TRAJ_SUCCESS(f) ((f) & (END_TRAJ|END_NEAR))

// Flags for "standard" trajectories
#define TRAJ_FLAGS_STD (END_TRAJ|END_BLOCKING|END_TIMER|END_ERROR | END_OBSTACLE)
#define TRAJ_FLAGS_SHORT_DISTANCE (END_TRAJ|END_ERROR|END_TIMER | END_BLOCKING)

// Flags for "cutting corners" trajectories.
#define TRAJ_FLAGS_NEAR (TRAJ_FLAGS_STD|END_NEAR)

// This enum is used for specifying a team color
typedef enum {YELLOW=0, RED} strat_color_t;

// This structure holds all the configuration data and state of the strategy
typedef struct  
{
    strat_color_t color; // Color of our robot
    // More to come...
} strat_info_t;

// This global var holds everything related to the strat
extern strat_info_t strat_info;

// Computes the symmetrical position depending on color
#define COLOR_Y(x) (strat_info.color == YELLOW ? (x) : 2000 - (x))

// Computes the symmetrical angle depending on color
#define COLOR_A(x) (strat_info.color == YELLOW ? (x) : -(x))

#define wait_traj_end(why) wait_traj_end_debug(why, __FILE__, __LINE__)

// Enum for strat speed
enum speed_e 
{
    CALAGE,
    SLOW,
    FUCKING_SLOW,
    FAST
};

void strat_begin( void );
int get_game_elapsed_time( void );
int strat_get_time( void );
void strat_timer_stop( void );

//============================================================================//
//================================== UTILS ===================================//
//============================================================================//

/*
 * Tests for end of trajectory.
 *
 * [in] why The allowed reasons for this function to return true.
 * Returns An error code indicating the reason of the end of the trajectory.
 */
int test_traj_end( int why );

/*
 * Waits for the end of a trajectory.
 *
 * [in] why The allowed reasons to end the trajectory.
 * Returns An error code indicating the reason of the end of the trajectory.
 */
int wait_traj_end_debug( int why, char *file, int line );

/*
 * Waits for some time.
 * [in] ms The time to wait, in milliseconds.
 */
void strat_wait_ms( int ms );

void strat_set_speed( enum speed_e speed );

#endif // _STRAT_H_
