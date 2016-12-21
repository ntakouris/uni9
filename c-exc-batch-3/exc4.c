#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

typedef struct Unit{
    char name[20];
    int atk;
    int cost;
    int sup;
    int code;
    int num;
} Unit;

int main(void){

    Unit * p1 = malloc(3 * sizeof(Unit));
    Unit * p2 = malloc(3 * sizeof(Unit));
    
    strcpy(p1->name, "Peon");
    p1->atk = 0;
    p1->cost = 20;
    p1->sup = -15;
    p1->code = 0;
    p1->num = 0;
    
    strcpy(p2->name, "Peon");
    p2->atk = 0;
    p2->cost = 20;
    p2->sup = -15;
    p2->code = 0;
    p2->num = 0;
    
    strcpy((p1+1)->name, "Grunt");
    (p1+1)->atk = 2;
    (p1+1)->cost = 30;
    (p1+1)->sup = 5;
    (p1+1)->code = 1;
    (p1+1)->num = 0;
    
    strcpy((p2+1)->name, "Grunt");
    (p2+1)->atk = 2;
    (p2+1)->cost = 30;
    (p2+1)->sup = 5;
    (p2+1)->code = 1;
    (p2+1)->num = 0;
    
    strcpy((p1+2)->name, "Troll");
    (p1+2)->atk = 3;
    (p1+2)->cost = 20;
    (p1+2)->sup = 10;
    (p1+2)->code = 2;
    (p1+2)->num = 0;
    
    strcpy((p2+2)->name, "Troll");
    (p2+2)->atk = 3;
    (p2+2)->cost = 20;
    (p2+2)->sup = 10;
    (p2+2)->code = 2;
    (p2+2)->num = 0;
    
    int money1 = 500;
    int money2 = 500;
    
    int i = 0;
    
    int rounds = 0;
    for(rounds=0;rounds<3;rounds++){
        int input;
        printf("Player 1's money: %d\n", money1);
        
        do{
                    printf("0 - Buy Peon (20)\n1 - Buy Grunt(30)\n3 - Buy Troll(20)\n4 - Next Player\n");

            scanf("%d", &input);
            if(input == 0){
                if(money1 > 20){
                    money1 -= 20;
                    p1->num++;
                }else{
                    printf("Not enough money\n");
                }
            }else if(input == 1){
                if(money1 > 30){
                    money1 -= 30;
                    (p1+1)->num++;
                }else{
                    printf("Not enough money\n");
                }
            }else if(input == 2){
                if(money1 > 20){
                    money1 -= 20;
                    (p1+2)->num++;
                }else{
                    printf("Not enough money\n");
                }
            }
        }while(input != 4);
        
        printf("Player 2's money: %d", money2);
        
        do{
                    printf("0 - Buy Peon (20)\n1 - Buy Grunt(30)\n3 - Buy Troll(20)\n4 - Next Player\n");

            scanf("%d", &input);
            if(input == 0){
                if(money2 > 20){
                    money2 -= 20;
                    p2->num++;
                }else{
                    printf("Not enough money\n");
                }
            }else if(input == 1){
                if(money2 > 30){
                    money2 -= 30;
                    (p2+1)->num++;
                }else{
                    printf("Not enough money\n");
                }
            }else if(input == 2){
                if(money2 > 20){
                    money2 -= 20;
                    (p2+2)->num++;
                }else{
                    printf("Not enough money\n");
                }
            }
        }while(input != 4);
        
        int peons1 = p1->num;
        money1 += peons1 * p1->sup;
        
        int grunts1 = (p1+1)->num;
        for(i=0;i<grunts1;i++){
            if(money1 > (p1+1)->sup){
                money1-=(p1+1)->sup;
            }else{
                (p1+1)->num--;
            }
        }
        
        int trolls1 = (p1+2)->num;
        for(i=0;i<trolls1;i++){
            if(money1 > (p1+2)->sup){
                money1-=(p1+2)->sup;
            }else{
                (p1+2)->num--;
            }
        }
        
        
        int peons2 = p2->num;
        money2 += peons2 * p2->sup;
        
        int grunts2 = (p2+1)->num;
        for(i=0;i<grunts2;i++){
            if(money2 > (p2+1)->sup){
                money2 -=(p2+1)->sup;
            }else{
                (p2+1)->num--;
            }
        }
        
        int trolls2 = (p2+2)->num;
        for(i=0;i<trolls2;i++){
            if(money2 > (p2+2)->sup){
                money2-=(p2+2)->sup;
            }else{
                (p2+2)->num--;
            }
        }
        
    }
    
    int att1 = 0;
    int att2 = 0;
    
    
    for(i=0;i<3;i++){
        att1 += (p1+i)->atk;
    }
    
    for(i=0;i<3;i++){
        att2 += (p2+i)->atk;
    }
    
    if(att1 == att2){
        if(att1 > att2){
            printf("Player 1 won with most attack power\n");
        }else{
            printf("Player 2 won with most attack power\n");
        }
    }else{
        if(money1 == money2){
            printf("Draw\n");
        }else if(money1 > money2){
            printf("Player 1 won with most money\n");
        }else{
            printf("Player 2 won with most money\n");
        }
    }
    
    return 0;
}