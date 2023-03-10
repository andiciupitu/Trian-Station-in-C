#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "station.h"


/* Creeaza o gara cu un numar fix de peroane.
 * 
 * platforms_no: numarul de peroane ale garii
 *
 * return: gara creata
 */
TrainStation* open_train_station(int platforms_no) {
     
     TrainStation *p;
    p = malloc(sizeof(TrainStation));
    p->platforms = malloc(platforms_no*sizeof(Train*));
    p->platforms_no = platforms_no;
    for( int i = 0; i <= platforms_no-1; i++)
    {
        p->platforms[i] = malloc(sizeof(Train));
        p->platforms[i]->locomotive_power = -1;
        p->platforms[i]->train_cars = NULL;
    }
    return p;
    
}


/* Elibereaza memoria alocata pentru gara.
 *
 * station: gara existenta
 */
void close_train_station(TrainStation *station) {
    
    if(station == NULL) return ;
    for(int i = 0; i <= station->platforms_no-1; i++)
    {
        TrainCar *q;
        station->platforms[i]->locomotive_power == -1;
        q = station->platforms[i]->train_cars;
        TrainCar *iter;
        while(q != NULL)
        {
           iter = q->next;
           free(q);
           q = iter;
        }
        free(station->platforms[i]);
    }
    free(station->platforms);
    free(station);
}


/* Afiseaza trenurile stationate in gara.
 *
 * station: gara existenta
 * f: fisierul in care se face afisarea
 */
void show_existing_trains(TrainStation *station, FILE *f) {
    
    if(station == NULL) return ;
    for(int i = 0; i <= station->platforms_no-1; i++ )
    {
        fprintf(f,"%d: ",i);
        TrainCar *q;
        q = station->platforms[i]->train_cars;
        if(station->platforms[i]->locomotive_power != -1)
        {
            fprintf(f,"(%d)",station->platforms[i]->locomotive_power);        
            while(q != NULL)
            {
                fprintf(f,"-|%d|",q->weight);
                q = q->next;
            }
        }
        fprintf(f,"\n");
    }
}


/* Adauga o locomotiva pe un peron.
 * 
 * station: gara existenta
 * platform: peronul pe care se adauga locomotiva
 * locomotive_power: puterea de tractiune a locomotivei
 */
void arrive_train(TrainStation *station, int platform, int locomotive_power) {

    if(station == NULL) return ;
    if(platform >= station->platforms_no || platform < 0) return ;
    if(station->platforms[platform]->locomotive_power != -1) return ;
    station->platforms[platform]->locomotive_power = locomotive_power;
}


/* Elibereaza un peron.
 * 
 * station: gara existenta
 * platform: peronul de pe care pleaca trenul
 */
void leave_train(TrainStation *station, int platform) {
    
    if(station == NULL) return ;
    if(platform >= station->platforms_no || platform < 0) return ;
    if(station->platforms == NULL) return ;
    if(station->platforms[platform] == NULL) return ;
    if(station->platforms[platform]->locomotive_power == -1) return ;

        TrainCar *q;
        station->platforms[platform]->locomotive_power = -1;
        q = station->platforms[platform]->train_cars;
        TrainCar *iter;
        while(q != NULL)
        {
           iter = q->next;
           free(q);
           q = iter;
        }
        station->platforms[platform]->train_cars = NULL;            
}


/* Adauga un vagon la capatul unui tren.
 * 
 * station: gara existenta
 * platform: peronul pe care se afla trenul
 * weight: greutatea vagonului adaugat
 */
void add_train_car(TrainStation *station, int platform, int weight) {
   
   if(station == NULL) return ;
   if(platform >= station->platforms_no || platform < 0) return ;
   TrainCar *q;
   q = malloc(sizeof(TrainCar));
   q->weight = weight;
   q->next = NULL;
   if(station->platforms[platform]->train_cars == NULL)
   {
       station->platforms[platform]->train_cars = q;
   }
   else
   {
       TrainCar *iter = station->platforms[platform]->train_cars;
       while(iter->next != NULL)
       {
           iter = iter->next;
       }
       iter->next = q;
   }
}


/* Scoate vagoanele de o anumita greutate dintr-un tren.
 * 
 * station: gara existenta
 * platform: peronul pe care se afla trenul
 * weight: greutatea vagonului scos
 */
void remove_train_cars(TrainStation *station, int platform, int weight) {
  
   if(station == NULL) return ;
   if(platform >= station->platforms_no || platform < 0) return ;
   if(station->platforms == NULL) return ;
   if(station->platforms[platform] == NULL) return ;
   if(station->platforms[platform]->locomotive_power == -1) return ;
   TrainCar *q;
   q = station->platforms[platform]->train_cars;
   if( q != NULL && q->weight == weight && q->next == NULL ){
       free(q);
       station->platforms[platform]->train_cars = NULL;
   }
   else{
       TrainCar *iter;
       
       while((q != NULL) && (q->weight == weight)){
           station->platforms[platform]->train_cars = q->next;
           free(q);
           q = station->platforms[platform]->train_cars;
       }
       TrainCar *prev;
       prev = NULL;
       while(q != NULL){
           TrainCar *iter;
          if(q->weight == weight ){
             iter = q->next;
             free(q);
             q = iter;
             if(prev != NULL)
             {
                 prev->next = q;
             }
          }
          else
          {
              prev = q;
              q = q->next;
              
          }
       }
   }
}


/* Muta o secventa de vagoane dintr-un tren in altul.
 * 
 * station: gara existenta
 * platform_a: peronul pe care se afla trenul de unde se scot vagoanele
 * pos_a: pozitia primului vagon din secventa
 * cars_no: lungimea secventei
 * platform_b: peronul pe care se afla trenul unde se adauga vagoanele
 * pos_b: pozitia unde se adauga secventa de vagoane
 */
void move_train_cars(TrainStation *station, int platform_a, int pos_a, 
                                int cars_no, int platform_b, int pos_b) {
    if(station == NULL) return ;
    if(pos_a <= 0 || pos_b <= 0) return ;
    if( cars_no <= 0) return;
    if(platform_a >= station->platforms_no || platform_a < 0) return;
    if(platform_b >= station->platforms_no || platform_b < 0) return;
    if(station->platforms == NULL) return ;
    if(station->platforms[platform_b] == NULL) return ;
    if(station->platforms[platform_a] == NULL) return ;
    if(station->platforms[platform_a]->train_cars == NULL) return ;
    if(station->platforms[platform_b]->train_cars == NULL && pos_b != 1) return ;
    if(station->platforms[platform_a]->locomotive_power == -1) return ;
    if(station->platforms[platform_b]->locomotive_power == -1) return ;
    
    if(pos_a == 1 && pos_b == 1)
    {
        TrainCar *ia, *copieb, *prev;
        int nr;
        ia = station->platforms[platform_a]->train_cars;
        copieb = station->platforms[platform_b]->train_cars;
        station->platforms[platform_b]->train_cars = station->platforms[platform_a]->train_cars;
        nr = cars_no;
        
        while(ia != NULL && nr > 0)
        {
            prev = ia;
            ia = ia->next;
            nr--;
        } 
        if( nr > 0)
        {
            station->platforms[platform_b]->train_cars = copieb;
            return;
        }
        else
        {
            prev->next = copieb;
        }
        station->platforms[platform_a]->train_cars = ia;
    }
    else
    {
        if(pos_a == 1 && pos_b > 1)
        {
            TrainCar *ia, *prevb, *ib, *preva;
            ia = station->platforms[platform_a]->train_cars;
            ib = station->platforms[platform_b]->train_cars;
            prevb = ib ;
            int NR = pos_b;
            while(NR > 1 && ib != NULL)
            {
                prevb = ib;  
                ib = ib->next;
                NR--;
            }
            if(NR == 1 && prevb != NULL)
            {
                prevb->next = station->platforms[platform_a]->train_cars;
                int nr = cars_no;
                while(ia != NULL && nr > 0)
                {
                    preva = ia;
                    ia = ia->next;
                    nr--;
                }
                station->platforms[platform_a]->train_cars = ia;
                preva->next = ib;
            }
        }
        else
        {
            if(pos_b == 1 && pos_a > 1)
            {
                TrainCar *ia, *preva, *copieb, *heada;
                int NR = pos_a;
                ia = station->platforms[platform_a]->train_cars;
                preva = ia;
                copieb = station->platforms[platform_b]->train_cars;
                while(NR > 1 && ia != NULL)
                {
                    heada = ia;
                    ia = ia->next;
                    NR--;
                }
                if(NR > 1) return;
                station->platforms[platform_b]->train_cars = ia;
                int nr = cars_no;
                while(ia != NULL && nr > 0)
                {
                    preva = ia;
                    ia = ia->next;
                    nr--;
                }
                if(nr > 0) return;
                heada->next = ia;
                preva->next = copieb;
            }
            else
            {

                TrainCar *ia, *preva, *ib, *prevb, *heada;
                ia = station->platforms[platform_a]->train_cars;
                heada = ia;
                preva = ia;
                int NR = pos_a;
                while(NR > 1 && ia != NULL)
                {
                    preva = ia;
                    NR--;
                    ia = ia->next;
                }   
                if(NR > 1)
                { return;}

                NR = pos_b;
                ib = station->platforms[platform_b]->train_cars;
                prevb = ib;
                while(ib != NULL && NR > 2)
                {
                    prevb = ib;
                    ib = ib->next;
                    NR--;
                }
                if(NR > 1)
                { return;}
                TrainCar *ibNext = ib->next;
                int nr = cars_no;
                TrainCar *a = ia;
                while( nr >= 1 && a != NULL)
                {
                    nr --;
                    a = a->next;
                }
                if(nr >= 0)
                {
                    return;
                }
                nr = cars_no;
                while (nr >= 1 && ia != NULL) {
                	ib->next = ia;
             		ib = ib->next;
                	ia = ia->next;
                	nr--;
                }
                if(nr > 1)
                {
                    return;
                }
                preva->next = ia;
                ib->next = ibNext;
            }
        }
    }                               
}


/* Gaseste trenul cel mai rapid.
 * 
 * station: gara existenta
 *
 * return: peronul pe care se afla trenul
 */
int find_express_train(TrainStation *station) {
    
    int max, sum, poz;
    max = -1;
    poz = -1;
   for(int i = 0; i <= station->platforms_no-1; i++)
   {
       TrainCar *q;
       sum = 0;
       q = station->platforms[i]->train_cars;
       while(q != NULL )
       {
           sum += q->weight;
           q = q->next;
       }
       if(max < station->platforms[i]->locomotive_power-sum)
       {
           max = station->platforms[i]->locomotive_power-sum;
           poz = i;
       }
   }
   return poz; 
}


/* Gaseste trenul supraincarcat.
 * 
 * station: gara existenta
 *
 * return: peronul pe care se afla trenul
 */
int find_overload_train(TrainStation *station) {

   int sum;  
   for(int i = 0; i <= station->platforms_no-1; i++)
   {
       TrainCar *q;
       sum = 0;
       q = station->platforms[i]->train_cars;
       while(q != NULL)
       {
           sum += q->weight;
           q = q->next;
       }
       if(station->platforms[i]->locomotive_power-sum < 0)
       {
           return i;
       }
   }
    return -1;
}


/* Gaseste trenul cu incarcatura optima.
 * 
 * station: gara existenta
 *
 * return: peronul pe care se afla trenul
 */
int find_optimal_train(TrainStation *station) {
    
    int min, sum, poz;
    min = 1000000000;
    poz = -1;
   for(int i = 0; i <= station->platforms_no-1; i++)
   {
       TrainCar *q;
       sum = 0;
       q = station->platforms[i]->train_cars;
       while(q != NULL )
       {
           sum += q->weight;
           q = q->next;
       }
       if((min > station->platforms[i]->locomotive_power-sum) && (station->platforms[i]->locomotive_power-sum > 0))
       {
           min = station->platforms[i]->locomotive_power-sum;
           poz = i;
       }
   }
   return poz; 
}


/* Gaseste trenul cu incarcatura nedistribuita bine.
 * 
 * station: gara existenta
 * cars_no: lungimea secventei de vagoane
 *
 * return: peronul pe care se afla trenul
 */
int find_heaviest_sequence_train(TrainStation *station, int cars_no, TrainCar **start_car) {
     
     if(station == NULL)
     {
         *start_car = NULL;
         return -1;
     }
     if(cars_no < 0) 
     {
         *start_car = NULL;
         return -1;
     }
     if(station->platforms == NULL) 
     {
          *start_car = NULL;
          return -1;
     }

     int sum, max = 0, poz = -1;
   for(int i = 0; i <= station->platforms_no-1; i++)
   {
       if(station->platforms[i] != NULL) 
       {
           TrainCar *q, *p;
           q = station->platforms[i]->train_cars;
           p = q;
           while(q != NULL && p!= NULL)
           {
                int nr = cars_no;
                sum = 0;
                p = q ;
                while(nr > 0 && p != NULL)
                {
                    sum += p->weight;
                    p = p->next;
                    nr--;
                }
                if(sum > max && nr == 0)
                {
                    max = sum;
                    poz = i;
                    *start_car = q;
                }
                q = q->next;
           }
       }  
   }
    if(poz != -1)
    {
        return poz;
    }
    else
    {
        *start_car = NULL;
        return -1;
    } 
}


/* Ordoneaza vagoanele dintr-un tren in ordinea descrescatoare a greutatilor.
 * 
 * station: gara existenta
 * platform: peronul pe care se afla trenul
 */
void order_train(TrainStation *station, int platform) {
    
    if (station == NULL)
        return;
    if (platform < 0 || platform >= station->platforms_no)
        return;
    int nr = 0, aux = 0;
    TrainCar *head = station->platforms[platform]->train_cars;
    TrainCar *second = NULL;
    if (station->platforms[platform]->locomotive_power == -1)
        return;
    for (head = station->platforms[platform]->train_cars; head->next != NULL; head = head->next)
    {
        for (second = head->next; second != NULL; second = second->next){
            if (head->weight < second->weight)
            {
                aux = head->weight;
                head->weight = second->weight;
                second->weight = aux;
            }
        }

    }
    return;
}



/* Scoate un vagon din trenul supraincarcat.
 * 
 * station: gara existenta
 */
void fix_overload_train(TrainStation *station) {
}