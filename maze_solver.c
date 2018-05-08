#include <libplayerc/playerc.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


#define pixels 480


typedef struct {

	playerc_client_t *client;
	
	playerc_position2d_t *pos;

	playerc_laser_t *laser;
	
	playerc_blobfinder_t *bf;

	playerc_gripper_t *gripper;

} Robot;


	
Robot *init_robot(int port) {
	Robot *new = (Robot *) malloc(sizeof(Robot));
	new->client = playerc_client_create(NULL, "localhost", (unsigned int) port);

	if(playerc_client_connect(new->client) != 0) {
		free(new);
		return NULL;
	}


	/*if (playerc_client_datamode (new->client, PLAYERC_DATAMODE_PULL) != 0) {
		fprintf(stderr, "error: %s\n", playerc_error_str());
 		return NULL;
	}
	if (playerc_client_set_replace_rule (new->client, -1, -1, PLAYER_MSGTYPE_DATA, -1, 1) != 0) {
		fprintf(stderr, "error: %s\n", playerc_error_str());
		return NULL;
	}*/
	
	new->pos = playerc_position2d_create(new->client, 0);

	if(playerc_position2d_subscribe(new->pos, PLAYERC_OPEN_MODE) != 0) {
		free(new);
		return NULL;
	}
	
	playerc_position2d_enable(new->pos,1);
	playerc_client_read(new->client);

	new->laser = playerc_laser_create(new->client, 0);
	if (playerc_laser_subscribe(new->laser, PLAYERC_OPEN_MODE)) {
		free(new);
    		return NULL;
	}

	
	new->bf = playerc_blobfinder_create(new->client, 0);
  	if (playerc_blobfinder_subscribe(new->bf, PLAYER_OPEN_MODE)){
		free(new);
    		return NULL;
	}

  	new->gripper = playerc_gripper_create(new->client, 0);
  	if (playerc_gripper_subscribe(new->gripper, PLAYER_OPEN_MODE)) {
		free(new);
		return NULL;
	}


	return new;
}


void destroy_robot(Robot **curr) {
	if(curr != NULL && *curr != NULL) {
		Robot *temp = *curr;

		playerc_gripper_unsubscribe(temp->gripper);
  		playerc_gripper_destroy(temp->gripper);
 		playerc_position2d_unsubscribe(temp->pos);
 		playerc_position2d_destroy(temp->pos);
		playerc_client_disconnect(temp->client);
 		playerc_client_destroy(temp->client);

		free(*curr);
		*curr = NULL;
	}
}


void coord_transform(double *x, double *y, int i, int j) {
	double half_pixels = pixels/2;
	
	
	i -= half_pixels;
	j -= half_pixels;

	*x = 25.0 * (double) j/half_pixels;
	*y = -25.0 * (double) i/half_pixels;
}

void go(Robot *a, double x, double y) {
		
	
	double vlong = 0.3, vrot = 0.0;	
	double dist;


	while(1) {
		playerc_client_read(a->client);
		dist = sqrt((x-a->pos->px)*(x-a->pos->px)+(y-a->pos->py)*(y-a->pos->py));
		
		if(dist < 1.0) {
			playerc_position2d_set_cmd_vel(a->pos, 0.3, 0, 0,1);
			return;
			
		}
		
		
		double dy = y - a->pos->py;
		double dx = x - a->pos->px;
			
		
		double m = sqrt(dx*dx + dy*dy);

		double dot = dx*cos(a->pos->pa) + dy*sin(a->pos->pa);
		
		double temp = dot/m;
		if(temp>1) temp = 1;
		if(temp < 0) temp = 0;
		
		double betha = acos(temp);
		
		
		if(dx*sin(a->pos->pa) - dy*cos(a->pos->pa) > 0) vrot = -betha;
		else vrot = betha;
		
		double campo_obst=0;
		int i;	
		for(i=180; i<=360; i+=30) 
			if (a->laser->scan[i][0] < 1.0)
				campo_obst += 2.0-a->laser->scan[i][0];

		for(i=175; i>=0; i-=30) 
			if (a->laser->scan[i][0] < 1.0)
				campo_obst -= 2.0-a->laser->scan[i][0];

		vrot -= 0.5 * campo_obst;

		
		//for(i = 175; i <= 185; ++i) if(a->laser->scan[i][0] < 0.4) vlong = 0.3;
		
		playerc_position2d_set_cmd_vel(a->pos, vlong, 0, vrot, 1);
	}

}

int main(int argc, char *argv[]) {
	Robot *robot = init_robot(6665);
	int n;
	int i, j, k;
	double x, y;
	
	
	scanf("%d", &n);
	
		
	for(k = 0; k < n; ++k) {
		scanf("%d %d", &i, &j);
		coord_transform(&x, &y, i, j);
		printf("coord: %.2lf %.2lf\n", x, y);
		go(robot, x, y);
	}
		
		
	destroy_robot(&robot);

	return 0;
}









