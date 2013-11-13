#include"Snake.h"

int auto_collide (Queue *s2, Queue *s1, coord auto_head);
int auto_dist_to_reward(coord auto_head, coord reward);
int auto_move(Queue *s1, Queue *s2, char *snake1dir, reward *rew, bomb *b, bombregion *br, Points* p, int TIME);
int play_one(Queue *s1, Queue *s2, coord *snake2, char *snake2dir, reward *r,Points* p);
void generate_perm(coord *new_head, reward *rew, int *min_distance, int ctr);



