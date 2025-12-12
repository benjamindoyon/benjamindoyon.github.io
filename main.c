//
//  main.c
//  hydrointegrodsv2
//
//  Created by Benjamin Doyon on 02/11/17.
//  Copyright 2017 King's College London. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include <math.h>
#include "/Users/benjamindoyon/principal/ProgrammesXcodeNew/parallel/parallel/parallel.h"
#define PI 3.14159265359

#define DENSITY 1
#define CURRENT 2
#define PARTDENSITY 4
#define PARTCURRENT 8
#define DMSOLUTION 16
#define LEFTCURRENT 32
#define RIGHTCURRENT 64
#define EXACTINITIALDENSITY 128
#define SPECTRAL 256
#define SPECTRALTWO 512

#define FULL 1
#define PART 2

#define TRUE 1
#define FALSE 0


char fullnameofthisfile[] = "/Users/benjamindoyon/principal/ProgrammesXcodeNew/hydrointegrods/hydrointegrods/main.c";
char pathofoutput[] = "/Users/benjamindoyon/principal/travail/projets/inprogress/projet86-hydrointegQNC/numer/resultsQNCcorrectSpectest/";
// pathofouput2 is used in "compare" procedure only.
char pathofoutput2[] = "/Users/benjamindoyon/principal/travail/projets/inprogress/projet86-hydrointegQNC/numer/resultsQNCcorrectSpeclarger2/";
int numberofthreads = 2;



double readtable(double *table, double speed);
double readdoubletable(double **table, double position, double speed);

double totaltime;


// Header is automatically extracted from this file and saved on a separate file in order to keep the information on the parameters generating the data.
// Extracted text starts at first occurence of triple-star and ends at second occurence of triple-star.

// explanations are as follows, more or less by paragraph:

// The min and max positions / speeds of the rods, and number of linear divisions in the position-speed grid.
// Rods will be distributed according to initspectraldensity: in each square of the grid, a rod is placed with probability initspectraldensity * dx * dv

// The min and max positions of the fluid cells (from which graph data are extracted), the min and max of velocities when calculating partial densities and currents to check eos, and the number of divisions within the fluid cells (essentially number of fluid cells). Averages are taken (of any given quantity) on all rods between x and x+dx as per these.

// the rod length - or minus the differential scattering phase

// sampling: average of particle and momentum densities are taken over all samples; in improved version, full fluid cell will be constructed out of all samples.
// if startingsample > 0: it loads files (particle and momentum density) and continue doing average assuming file loaded had averaged over sartingsample samples.

// time evolution: outputs start at initialtime, increase by timestep for numberofsteps.

// name of output file. to this is appended the step number and things like "-density" or "-current". extension .dat is automatically appended.

// FROMINITIALGGE : calculate initial spectral density rho_p from an initial GGE
// ZEROTEMPERATURE : if FROMINITIALGGE is on, calculate the density at zero temperature (the initial GGE given is that with temperature scaled out)
// ACCELERATION : include an acceleration (external force field)
// SOFTRODS : flea-gas procedure. Otherwise: standarx hard rods.
// LOOP : do the evolution loop, either for proper evolution or for checking with exact domain-wall solution (in hard-rod case)
// EVOLVE : do time evolution.
// GRAPH : in evolution loop, create data lists. outputtype set to DENSITY and/or CURRENT and/or PARTDENSITY and/or PARTCURRENT and/or DMSOLUTION.
// DRAW : in time evolution, output a "picture" of trajectories - it's a text file with "X" where there's a particle and " " where there isn't.
// EOS : check equations of state by calculating integrated euler equation over a region. eulerleft, eulerright are left / right positions of region, eulerrange is total time of region. Checks all regions from initialtime all the way to initialtime + timestep*numberofsteps - eulerrange.
// EOSPARTIAL : same check, but with partial densities and currents, instead of full velocity integrated ones, thus requiring proper EOS.
// VEFF : if EOSPARTIAL is on, calculate the veff curve for all times at position eulerleft. if EVOLVE is on, calculate velocity of test particles in fluid cell at eulerleft, as integrated from initial time to last time.
// OUTPUTOCCUPATION : output the initial occupation number at position occposition (using the true initial density of particle, including Bragg if any).
// RETRIEVEFERMIPOINTS : ...
// BRAGGPULSE : ...
// SOLUTIONBYITERATION : ...
// TEXFILEONLY : ...
// RMSSPECTRALCHANGE : ...
// BRANCHING : ...
// CONSERVEDQTYONLY : ...
// PERFORMANCE : ...
// PERFORODTABLE
// PERFORODTABLEDIFF
// RECOMBINEONLY
// DISPLAYMAXDISPLACEMENT

// Everywhere: one can use global variable totaltime in order to modify acceleration / interaction with respect to time (except when using PERFORMANCE).



// BEGIN HEADER ***


// lengthscale times larger
// real QNC: all distances divided by lengthscale in micrometer, all times divided by lengthscale in micrometer^{2} (mass=1)
// thermal, harmonic
// note: omega is in micrometer^{-2}, which is the unit of energy.

#define lengthscale 8

double omegasquare = 0.423/lengthscale/lengthscale, omega=0.65/lengthscale, length = 120*lengthscale, mu = 100, ccoupling = 12.3;
double Momegasquare = 0.423/lengthscale/lengthscale;

//#define CONSERVEDQTYONLY
//#define TEXFILEONLY
#define FROMINITIALGGE
//#define ZEROTEMPERATURE
//#define OUTPUTOCCUPATION
#define BRAGGPULSE
#define ACCELERATION
#define SOFTRODS
#define LOOP
#define EVOLVE
#define GRAPH
#define PERFORMANCE
#define PERFORODTABLE
#define PERFORODTABLEDIFF
//#define DISPLAYMAXDISPLACEMENT

double initposmin=-30*lengthscale, initposmax=30*lengthscale, initspeedmin=-50, initspeedmax=50;
double rangeposmin=-30*lengthscale, rangeposmax=30*lengthscale, rangespeedmin=-25, rangespeedmax=25;
double graphposmin=-70*lengthscale, graphposmax=70*lengthscale, partialspeedmin=-1, partialspeedmax=1;
double graphspeedmin=-50, graphspeedmax=50;
int divisions=5000, rangedivisions=200, graphdivisions=70, graphspeeddivisions=70;

#ifdef PERFORMANCE
double perfoposmin = -100*lengthscale, perfoposmax = 100*lengthscale, perfospeedmin = -100, perfospeedmax = 100;
int perfodivisions = 15000;
#endif

#ifdef TEXFILEONLY
char additionallineinit[] = "\\begin{center}\n\n";
char linefortexoutputdensity[] = "title=density , xlabel={$x$}, ymax=15";
char additionallinedensity[] = "\n\n";
char linefortexoutputcurrent[] = "title=current , xlabel={$x$}, ymin=-150,ymax=150";
char additionallinecurrent[] = "\n\n";
char additionallinefin[] = "\\end{center}\n\n";
char scalefortexoutput[] = "1.5";
char linefortexoutputspectral[] = "title=spectraldensity, xlabel={$x$}, ylabel={$v$}";
char pointsizescatterplot[] = "1pt";
char additionallinespectral[] = "\n\n";
int texoutputskip = 10;
#endif

#ifdef FROMINITIALGGE
// needs to be defined only if FROMINITIALGGE is turned on
double initialgge (double x, double v) {
    return (v*v/2 + Momegasquare*x*x/2 - mu)/10;
}
#endif

#ifndef FROMINITIALGGE
// needs to be defined only if FROMINITIALGGE is turned off
double initspectraldensity (double x, double v) {
    return 0.5*exp(-x*x/10000)*(exp(-(v-4)*(v-4))+exp(-(v+4)*(v+4)));
}
#endif

#ifdef BRAGGPULSE
// the pulse will create shifts in velocities from - braggpulsnumberofharmonics * braggpulseshift to braggpulsnumberofharmonics * braggpulseshift passing by 0.
int braggpulsenumberofharmonics=1; 
double braggpulseshift=25;
double braggpulseamplitude[3] = {0.5,0,0.5};
#endif

double momentumprime (double v) {
    return 1;
}

double rodlength(double v1, double v2) {
    return -2*ccoupling / ( (v1-v2)*(v1-v2) + ccoupling*ccoupling ); // / momentumprime(v1);
}

double crossingscale = 2;

#ifdef ACCELERATION
// need to define maximumdeltat and acceleration just if ACCELERATION is turned on
double maximumdeltat = 0.005*lengthscale;
double acceleration(double x) {
    return -Momegasquare*x;
}
#endif

// with timetoforget = 3 we have that collisions of particles both with small velocities < 0.3 will not be treated correctly. This is 1-2% of range.
// so about 0.02^2 = 0.0004 proportion of collisions incorrect

int startingsample = 0;
int totalnumberofsamples = 5;

// period = 9.66 = 2 pi / omega with imprecision 8 E-5. More precise: 9.6607242.
// 1/5th of a period: with 1.93 error is 2 E-3, more precise number is 1.932145.

int numberofsteps = 50;
double initialtime = 0;
double timestep = 1.93*lengthscale; // this is 1/5th of a period (period = 9.66 = 2 pi / omega)

char thename[30] = "realQNCthspectest3";

#ifdef OUTPUTOCCUPATION
double occposition=0;
#endif

int outputtype = DENSITY+SPECTRAL;
double eulerleft = -800, eulerright = 200, eulerrange = 0;

int numberiterations = 100; // for calculation of veff and other TBA quantities
double tolerance = 0.0001; // percentage of tolerance for calculation of TBA quantities: a message appears if broken, with the value of relative error.






// *** END HEADER



// =============== Some basic stuff =================

#ifdef PERFORMANCE
double perfodeltax, perfodeltav;
#ifdef ACCELERATION
double *accelerationtable;
#endif
double **rodlengthtable;
double *rodlengthtablediff;
#endif

#ifndef ACCELERATION
double timetoforget = 200000;
double maximumdeltat = 2000;
#endif

#ifdef FROMINITIALGGE
double **thetablesd=NULL;
double initspectraldensity (double x, double v) {
    return readdoubletable(thetablesd,x,v);
}
#endif

double preinitspectraldensity (double x, double v) {
#ifdef BRAGGPULSE
    double val=0;
    int n=-braggpulsenumberofharmonics;
    while (n<=braggpulsenumberofharmonics) {
        val += braggpulseamplitude[n+braggpulsenumberofharmonics] * initspectraldensity(x, v+n*braggpulseshift);
        n++;
    }
    return val;
#else
    return initspectraldensity(x, v);
#endif
}

#ifdef SOLUTIONBYITERATION
double **exactoccupation;
double energyprime (double v) {
    return v*momentumprime(v);
}
#endif

#ifdef OUTPUTCORRELATION
double densityrawinit,densityaverageinit;
double *tableinitial;
#endif


double smalldisplacement;

double *occupationfct=NULL;

int Debug = 0;

struct collisionnode {
    struct rod *rod1;
    struct rod *rod2;
    double timetocollision;
    struct collisionnode *parent;
    struct collisionnode *lesser;
    struct collisionnode *greater;
};

struct collisionnode *collisiontree=NULL;

#ifdef SOFTRODS

struct rod {
    double pos;
    double speed;
    int id;
    int *tableactiverods;
    struct rod *prev;
    struct rod *next;
    struct collisionnode *collisionnode;
    double localtime;
    int touched;
};

struct rod **tabletestpartid;
double *tabletestpartpos;
int sizetabletestpart;

struct activepair {
    struct rod *rod1;
    struct rod *rod2;
    struct activepair *prev;
    struct activepair *next;
};
struct activepair *activepairs=NULL;

#else

struct rod {
    double pos;
    double speed;
    struct rod *next;
};

#endif

struct rod *rods=NULL;

double deltax, deltav, rangedeltav, rangedeltax, graphdeltav, graphdeltax, graphdeltan;
int totalnumber;


#ifdef BRANCHING
struct rod *savedrods=NULL;
double savedtotaltime;
#endif

double getposition(int m) {
    return initposmin+m*deltax+deltax/2;
}

double getspeed(int m) {
    return initspeedmin+m*deltav+deltav/2;
}

double getrangeposition(int m) {
    return rangeposmin+m*rangedeltax+rangedeltax/2;
}

double getrangespeed(int m) {
    return rangespeedmin+m*rangedeltav+rangedeltav/2;
}

double getgraphposition(int m) {
    return graphposmin+m*graphdeltax+graphdeltax/2;
}

double getgraphspeed(int m) {
    return graphspeedmin+m*graphdeltav+graphdeltav/2;
}

double getoccupationtablevalue(int m) {
    return 0+m*graphdeltan+graphdeltan/2;
}

#ifdef PERFORMANCE
double getperfoposition(int m) {
    return perfoposmin+m*perfodeltax+perfodeltax/2;
}

double getperfospeed(int m) {
    return perfospeedmin+m*perfodeltav+perfodeltav/2;
}

#endif


int getpositionindex(double x) {
    return (int)((x-initposmin)/deltax);
}

int getspeedindex(double x) {
    return (int)((x-initspeedmin)/deltav);
}

int getrangepositionindex(double x) {
    return (int)((x-rangeposmin)/rangedeltax);
}

int getrangespeedindex(double x) {
    return (int)((x-rangespeedmin)/rangedeltav);
}

int getgraphpositionindex(double x) {
    return (int)((x-graphposmin)/graphdeltax);
}

int getgraphspeedindex(double x) {
    return (int)((x-graphspeedmin)/graphdeltav);
}

int getoccupationtableindex(double x) {
    return (int)(x/graphdeltan);
}

#ifdef PERFORMANCE
int getperfopositionindex(double x) {
    return (int)((x-perfoposmin)/perfodeltax);
}

int getperfospeedindex(double x) {
    return (int)((x-perfospeedmin)/perfodeltav);
}
#endif

#ifdef PERFORMANCE
#ifdef ACCELERATION
double readaccelerationtable(double x) {
    int n = getperfopositionindex(x);
    if (n<0 || n>=perfodivisions) return acceleration(x);
    else return accelerationtable[n];
}
#endif
#ifdef PERFORODTABLE
#ifndef PERFORODTABLEDIFF
double readrodlengthtable(double v1, double v2) {
    int m = getperfospeedindex(v1);
    int n = getperfospeedindex(v2);
    if (m<0 || m>=perfodivisions || n<0 || n>=perfodivisions) return rodlength(v1, v2);
    return rodlengthtable[m][n];
}
#else
double readrodlengthtable(double v1, double v2) {
    int m = getperfospeedindex(v1-v2);
    if (m<0 || m>=perfodivisions) return rodlength(v1, v2);
    return rodlengthtablediff[m];
}
#endif
#endif
#endif


double readtable(double *table, double speed) {
    int n = getrangespeedindex(speed);
    if (n<0 || n>rangedivisions) return 0;
    return table[n];
}

double readdoubletable(double **table, double position, double speed) {
    int m = getrangepositionindex(position);
    int n = getrangespeedindex(speed);
    if (m<0 || m>=rangedivisions || n<0 || n>=rangedivisions) return 0;
    return table[m][n];
}


// ============== Collision tree procedures ==============

int sidecounter=0;

void freecollisiontree(struct collisionnode *c) {
    if (!c) return;
    freecollisiontree(c->lesser);
    freecollisiontree(c->greater);
}

struct collisionnode *createcollisionnode (struct rod *q, double timetocollision, struct collisionnode *parent) {
    struct collisionnode *c = (struct collisionnode *)malloc(sizeof(struct collisionnode));
    c->rod1 = q;
    c->rod2 = q->next;
    c->timetocollision = timetocollision;
    c->lesser = NULL;
    c->greater = NULL;
    c->parent = parent;
    q->collisionnode = c;
    return c;
}

void addcollisionnode(struct rod *q, double timetocollision) {
    struct collisionnode *c=collisiontree, *previous=NULL;
    while (c) {
        previous = c;
        if (timetocollision > c->timetocollision) c=c->greater;
        else c=c->lesser;
    }
    if (!previous) {
        collisiontree = createcollisionnode(q, timetocollision, NULL);
    } else {
        if (timetocollision > previous->timetocollision) previous->greater = createcollisionnode(q, timetocollision, previous);
        else previous->lesser = createcollisionnode(q, timetocollision, previous);
    }
}

void deletecollisionnode (struct collisionnode *c) {
    struct collisionnode *d;
    if (!c) return;
    c->rod1->collisionnode = NULL;
    if (!c->greater) {
        if (!c->parent) {
            collisiontree = c->lesser;
            if (c->lesser) c->lesser->parent = NULL;
        } else {
            if (c->parent->lesser == c) c->parent->lesser = c->lesser;
            else c->parent->greater = c->lesser;
            if (c->lesser) c->lesser->parent = c->parent;
        }
    } else if (!c->lesser) {
        if (!c->parent) {
            collisiontree = c->greater;
            c->greater->parent = NULL;
        } else {
            if (c->parent->lesser == c) c->parent->lesser = c->greater;
            else c->parent->greater = c->greater;
            c->greater->parent = c->parent;
        }
    } else if ((sidecounter++)%2) {
        if (!c->parent) {
            collisiontree = c->lesser;
            c->lesser->parent = NULL;
        } else {
            if (c->parent->lesser == c) c->parent->lesser = c->lesser;
            else c->parent->greater = c->lesser;
            c->lesser->parent = c->parent;
        }
        d=c->lesser;
        while (d->greater) {
            d=d->greater;
        }
        d->greater = c->greater;
        c->greater->parent = d;
    } else {
        if (!c->parent) {
            collisiontree = c->greater;
            c->greater->parent = NULL;
        } else {
            if (c->parent->lesser == c) c->parent->lesser = c->greater;
            else c->parent->greater = c->greater;
            c->greater->parent = c->parent;
        }
        d=c->greater;
        while (d->lesser) {
            d=d->lesser;
        }
        d->lesser = c->lesser;
        c->lesser->parent = d;
    }
    free(c);
}

// ============== Evolution procedures ================


struct rod *minrodaffected, *maxrodaffected;

// calculate time of intersection if positive. Assume r1 is to the left of r2.
// for hard rods: time of intersection determined by length of rod. p2 + t*v2 - (p1 + t*v1) = a(v1,v2) => t = (a(v1,v2) + p1 - p2) / (v2-v1)
// for soft rods: look for interesection of the centers only. length of rods is involved only in recoil / advanced performed at intersection with unlinked rods.
// considering local times t1 and t2, formula is: p2 - v2*t2 + s*v2 - (p1 - v1*t1 + s*v1) = 0 => s = (p2 - v2*t2 - p1 + v1*t1) / (v1-v2)

double intersect(struct rod *r1, struct rod *r2) {
    if (!r1 || !r2) return -1;
    if (r1->speed <= r2->speed) return -1;
#ifdef SOFTRODS
    return (r1->pos - r1->speed * r1->localtime - r2->pos + r2->speed * r2->localtime) / (r2->speed - r1->speed);    
#else
    return (rodlength(r1->speed,r2->speed) + r1->pos - r2->pos) / (r2->speed - r1->speed);
#endif
}

#ifdef SOFTRODS

void newactivepair (struct rod *q, struct rod *p) {
//    printf("new: (%d,%d)\n",q->id,p->id);
    activepairs->prev = (struct activepair *)malloc(sizeof(struct activepair));
    activepairs->prev->next = activepairs;
    activepairs = activepairs->prev;
    activepairs->rod1 = q;
    activepairs->rod2 = p;
}


void moveintime (struct rod *q, double time) {
    q->pos += time * q->speed;
#ifdef ACCELERATION
#ifdef PERFORMANCE
    q->speed += time * readaccelerationtable(q->pos);
#else
    q->speed += time * acceleration(q->pos);
#endif
#endif
}

// we assume q is before p in the chain. we take care of case where q and p are adjacent.
void exchange(struct rod *q, struct rod *p) {
    if (q==p) return;
    struct rod *a, *b;
    if (q->prev) q->prev->next = p; else rods = p;
    if (p->next) p->next->prev = q;
    a=p->prev;
    b=q->next;
    p->prev = q->prev;
    q->next = p->next;
    if (b==p) p->next = q; else {b->prev = p; p->next = b;}
    if (a==q) q->prev = p; else {a->next = q; q->prev = a;}
}

void movebackwardrecursive(struct rod *q, double length);
void moveforwardrecursive(struct rod *q, double length);

#ifdef DISPLAYMAXDISPLACEMENT
double maxdisplacement=0;
#endif

void performcollision(struct rod *q) {
    struct rod *p=q->next;
    double length;
//    printf ("- %d.%d: ",q->id,p->id);
    if (!(q->tableactiverods[p->id])) {
        q->tableactiverods[p->id] = 1;
        p->tableactiverods[q->id] = 1;
        newactivepair(q,p);
#ifdef PERFORMANCE
#ifdef PERFORODTABLE
        length = readrodlengthtable(q->speed, p->speed);
#else
        length = rodlength(q->speed, p->speed);
#endif
#else
        length = rodlength(q->speed, p->speed);
#endif
#ifdef DISPLAYMAXDISPLACEMENT
        double posinit = q->pos;
#endif
        if (length>=0) {
            moveforwardrecursive(q,length);
            movebackwardrecursive(p,length);
        } else {
//            printf("<->\n");
            movebackwardrecursive(q,-length);
            moveforwardrecursive(p,-length);
        }
#ifdef DISPLAYMAXDISPLACEMENT
        if (fabs(q->pos - posinit) > maxdisplacement) {
            maxdisplacement = fabs(q->pos - posinit);
        }
#endif
    } else {
//        printf("><\n");
        exchange(q, p);
        if (minrodaffected == q) minrodaffected = p;
        if (maxrodaffected == p) maxrodaffected = q;
    }
}

void moveforwardrecursive(struct rod *q, double length) {
    double l=length;
    while (q->next) {
        if (!q->next->touched) {
            moveintime(q->next, q->localtime - q->next->localtime);
//            q->next->pos += (q->localtime - q->next->localtime) * q->next->speed;
            q->next->localtime = q->localtime;
            q->next->touched = 1;
        }
        if (maxrodaffected == q) maxrodaffected = q->next;
if (q->next->pos < q->pos) printf("AAAAA\n");
        if (q->next->pos < q->pos+l) {
            l -= (q->next->pos - q->pos);
            q->pos=q->next->pos;
            performcollision(q);
        } else break;
    }
    q->pos += l;
}

void movebackwardrecursive(struct rod *q, double length) {
    double l=length;
    while (q->prev) {
        if (!q->prev->touched) {
            moveintime(q->prev, q->localtime - q->prev->localtime);
//            q->prev->pos += (q->localtime - q->prev->localtime) * q->prev->speed;
            q->prev->localtime = q->localtime;
            q->prev->touched = 1;
        }
        if (minrodaffected == q) minrodaffected = q->prev;
if (q->prev->pos > q->pos) printf("BBBBBB\n");
            if (q->prev->pos > q->pos-l) {
            l -= (q->pos - q->prev->pos);
            q->pos=q->prev->pos;
            performcollision(q->prev);
        } else break;
    }
    q->pos -= l;
}


#endif

int checkforcollisions (struct rod *init, struct rod *finnotincluded, double period) {
    double s;
    int n=0;
    while (init && init->next != finnotincluded) {
        s = intersect(init,init->next);
        if (s >= 0 && s < period) {
            addcollisionnode(init, s);
//            printf("%d,%d\n",init->id,init->next->id);
            n++;
        }
        init=init->next;
    }
    return n;
}

// evolve the rods for time t.
void evolve (double t) {
    double forcelessperiod;
    struct collisionnode *c;
    if (!rods) return;
    struct rod *q, *p;
    double diff;
    struct activepair *a, *b;
    int n,m;
    while (t>0) {
//        printf("period\n");
        if (t>maximumdeltat) forcelessperiod = maximumdeltat;
        else forcelessperiod = t;

//        printf("check\n");
        // check for collisions. Build collisiontree. It should start empty at this point.
        n=checkforcollisions(rods, NULL, forcelessperiod);

//        printf("%d: ..........collide.........\n",n);
        // for every collision node, perform collision and reorganize tree. Evolve rods involved in collisions.
        m=0;
        while (collisiontree) {
//            printf("rest: %d\n",n);
            m++;
            c = collisiontree;
//            m=0;
            while (c->lesser) {
                c=c->lesser;
//                m++;
            }
//            printf("%d/%d = %lf; %lf\n",m,n,1.0*m/n,1.0*m/log(n));
//            printf("ttc: %lf\n",c->timetocollision);
/*            
            q=rods;
            while (q && q->next) {
                diff = intersect(q, q->next);
                if (diff>=0 && diff < c->timetocollision && c->rod1 != q) printf("--- not minimal: %d.%d [%lf]\n",q->id,q->next->id,diff);
                if (q->localtime > q->next->localtime) {
                    f = q->pos;
                    g = q->next->pos + (q->localtime - q->next->localtime)*q->next->speed;
                } else {
                    f = q->pos + (q->next->localtime - q->localtime)*q->speed;
                    g = q->next->pos;
                }
                if (f > g+0.00001) printf("-- %lf,%d --- not ordered: %d(%lf).%d(%lf)\n",totaltime,m,q->id,f,q->next->id,g);
                q=q->next;
            }
*/
            moveintime(c->rod1, c->timetocollision - c->rod1->localtime);
            moveintime(c->rod2, c->timetocollision - c->rod2->localtime);
//            c->rod1->pos += (c->timetocollision - c->rod1->localtime) * c->rod1->speed;
//            c->rod2->pos = c->rod1->pos;
//            printf("%lf\n",c->rod2->pos - c->rod1->pos);
//            printf("%lf\n",(c->rod2->pos + (c->timetocollision - c->rod2->localtime) * c->rod2->speed) - c->rod1->pos);
//            if (c->rod2 != c->rod1->next) printf("problem\n");
            c->rod1->localtime = c->timetocollision;
            c->rod2->localtime = c->timetocollision;
            c->rod1->touched = 1;
            c->rod2->touched = 1;
            minrodaffected = c->rod1;
            maxrodaffected = c->rod2;
//            printf("%d(%lf),%d(%lf): %lf\n",c->rod1->id,c->rod1->pos,c->rod2->id,c->rod2->pos,c->timetocollision);
            performcollision(c->rod1);
            if (minrodaffected->prev) q=minrodaffected->prev;
            else q=minrodaffected;
            while (q != maxrodaffected->next) {
//                printf("gr: %d(%lf)[%lf]\n",q->id,q->pos,q->localtime);
                if (q->collisionnode) {
//                    printf("del %d,%d\n",q->collisionnode->rod1->id,q->collisionnode->rod2->id);
//                    printf("par %d, less %d, great %d\n",q->collisionnode->parent, q->collisionnode->lesser, q->collisionnode->greater);
                    deletecollisionnode(q->collisionnode);n--;}
                q->touched = 0;
                q=q->next;
            }
            if (minrodaffected->prev) q=minrodaffected->prev;
            else q=minrodaffected;
            if (maxrodaffected->next) p=maxrodaffected->next;
            else p = maxrodaffected;
//            printf("add:\n");
            n+=checkforcollisions(q, p->next, forcelessperiod);
        }
        
        // evolve remaining rods, taking care of their local time (their proper time within the present period of forceless evolution)
        totaltime += forcelessperiod;
        t -= maximumdeltat;
        q=rods;
        while (q) {
            moveintime(q, forcelessperiod-q->localtime);
            q->localtime = 0;
            q=q->next;
        }

        // desactivate the pairs that have wandered too far from each other.
        // Recall that the chain finishes with a dummy node, there for convenience; and that the first node does not have ->prev set to NULL.
        a = activepairs;
        a->prev = NULL;
        while (a->next) {
            b=a->next;
            diff = a->rod1->pos - a->rod2->pos;
            if (diff > crossingscale || diff < -crossingscale) {
                a->rod1->tableactiverods[a->rod2->id] = 0;
                a->rod2->tableactiverods[a->rod1->id] = 0;
                if (a->prev) a->prev->next = a->next;
                else activepairs = a->next;
                a->next->prev = a->prev;
                free(a);
            }
            a=b;
        }
    }
}


// =================== Sampling procedure ==================


// a random number between 0 and 1
double pseudorandom (void) {
    return ((double)rand())/RAND_MAX;
}

// produces a sample as a chain with root "rods". if "rods" was attributed, erase it. output: the number of rods.
int sample (void) {
    int m,q,n;
    struct rod *r,*prev, *s, *sprev;
    struct activepair *a;
    
    // erase if already exists
    while (rods) {
        r=rods->next;
#ifdef SOFTRODS
        free(rods->tableactiverods);
#endif
        free(rods);
        rods = r;
    }
    // erase the table of linked (active) pairs if it exists
#ifdef SOFTRODS
    while (activepairs) {
        a = activepairs->next;
        free(activepairs);
        activepairs=a;
    }
#endif
    if (collisiontree) freecollisiontree(collisiontree);
    // set total time to 0: this is the start of the evolution
    totaltime = 0;
    
    // sample: each cell is sampled according to density, and if a particle is put, it is put in a random place within the cell
    rods=NULL;
    r=NULL;
    n=0;
    m=0;
    while (m<divisions) {
        q=0;
        while (q<divisions) {
            if ( pseudorandom() < preinitspectraldensity(getposition(m), getspeed(q)) * deltax * deltav) {
                if (r) {
                    r->next = malloc(sizeof(struct rod));
                    prev=r;
                    r=r->next;
                } else {
                    rods = malloc(sizeof(struct rod));
                    prev=NULL;
                    r=rods;
                }
                r->pos = getposition(m)-deltax/2+deltax*pseudorandom();
                r->speed = getspeed(q)-deltav/2+deltav*pseudorandom();
#ifdef SOFTRODS
                r->id = n;
                r->tableactiverods = NULL;
                r->prev = prev;
                r->localtime = 0;
                r->collisionnode = NULL;
                r->touched = 0;
#endif
                r->next = NULL;
                n++;
            }
            q++;
        }
        m++;
    }
    // at exit of this loop: n is the number of rods.

#ifdef SOFTRODS
    // make tables of active rods: this is the table of links between rods that have collided. Here, no collision yet.
    s=rods;
    while (s) {
        s->tableactiverods = malloc(sizeof(int)*n);
        m=0;
        while (m<n) {
            s->tableactiverods[m]=0;
            m++;
        }
        s=s->next;
    }
    // make the corresponding chain of active pairs.
    activepairs = malloc(sizeof(struct activepair));
    activepairs->rod1=NULL;
    activepairs->rod2=NULL;
    activepairs->next=NULL;
    activepairs->prev=NULL;
#endif
    // re-order in order of increasing positions
    s=rods;
    sprev=NULL;
    while (s) {
        r=s;
        prev = sprev;
        s=NULL;
        while (r->next)  {
            if (r->next->pos < r->pos) {
                if (prev) {
                    if (!s) sprev=prev;
#ifdef SOFTRODS
                    exchange(r,r->next);
#else
                    prev->next = r->next;
                    r->next = r->next->next;
                    prev->next->next = r;
#endif
                    prev=prev->next;
                } else {
                    if (!s) sprev = NULL;
#ifdef SOFTRODS
                    exchange(r,r->next);
#else
                    rods = r->next;
                    r->next = r->next->next;
                    rods->next = r;
#endif
                    prev = rods;
                }
                if (!s) s=prev;
            } else {
                prev=r;
                r=r->next;
            }
        }
    }
    return n;
}



// =================== Input procedure ====================



FILE *openfile(char *s, char *m) {
    char name[300];
    if (_para_currentthread==0) sprintf(name, "%s%s.dat",pathofoutput,s);
    else sprintf(name, "%s%s.dat",_para_temporary_deposit[_para_currentthread],s);
    return fopen(name,m);
}

FILE *openfile2(char *s, char *m) {
    char name[300];
    sprintf(name, "%s%s.dat",pathofoutput2,s);
    return fopen(name,m);
}

double *inputleftfluidcell(char *name, int *number) {
    double *table;
    char name2[300];
    FILE *p;
    int m;
    sprintf(name2, "%s-leftfluidcellsize",name);
    p=openfile(name2,"r");
    if (p) {fscanf(p,"%d",number);fclose(p);}
    else *number=0;
    table = malloc(*number*sizeof(double));
    if (!table) return NULL;
    sprintf(name2, "%s-leftfluidcell",name);
    p=openfile(name2,"r");
    if (p) {
        m=0;
        while (m<*number) {
            fscanf(p,"%lf",table+m);
            m++;
        }
        fclose(p);
    }
    return table;
}

double *inputrightfluidcell(char *name, int *number) {
    double *table;
    char name2[300];
    FILE *p;
    int m;
    sprintf(name2, "%s-rightfluidcellsize",name);
    p=openfile(name2,"r");
    if (p) {fscanf(p,"%d",number);fclose(p);}
    else *number=0;
    table = malloc(*number*sizeof(double));
    if (!table) return NULL;
    sprintf(name2, "%s-rightfluidcell",name);
    p=openfile(name2,"r");
    if (p) {
        m=0;
        while (m<*number) {
            fscanf(p,"%lf",table+m);
            m++;
        }
        fclose(p);
    }
    return table;
}

void input(char *name, int type, double *tabledensity, double *tablecurrent, double *tablespecmin, double *tablespecmax, double *tablepartdensity, double *tablepartmoment, double *tablepartcurrent) {
    char name2[300];
    FILE *p;
    int m;
    if (type&DENSITY && tabledensity) {
        sprintf(name2, "%s-density",name);
        p=openfile(name2,"r");
        if (p) {
            m=0;
            while (m<graphdivisions) {
                fscanf(p,"%*lf %lf",tabledensity+m);
                m++;
            }
            fclose(p);
        }
    }
    if (type&CURRENT && tablecurrent) {
        sprintf(name2, "%s-current",name);
        p=openfile(name2,"r");
        if (p) {
            m=0;
            while (m<graphdivisions) {
                fscanf(p,"%*lf %lf",tablecurrent+m);
                m++;
            }
            fclose(p);
        }
    }
    if (type&SPECTRALTWO && tablespecmin) {
        sprintf(name2, "%s-spectralmin",name);
        p=openfile(name2,"r");
        if (p) {
            m=0;
            while (m<graphdivisions) {
                fscanf(p,"%*lf %lf",tablespecmin+m);
                m++;
            }
            fclose(p);
        }
    }
    if (type&SPECTRALTWO && tablespecmax) {
        sprintf(name2, "%s-spectralmax",name);
        p=openfile(name2,"r");
        if (p) {
            m=0;
            while (m<graphdivisions) {
                fscanf(p,"%*lf %lf",tablespecmax+m);
                m++;
            }
            fclose(p);
        }
    }
    if (type&PARTDENSITY && tablepartdensity) {
        sprintf(name2, "%s-partdensity",name);
        p=openfile(name2,"r");
        if (p) {
            m=0;
            while (m<graphdivisions) {
                fscanf(p,"%*lf %lf",tablepartdensity+m);
                m++;
            }
            fclose(p);
        }
    }
    if (type&PARTCURRENT && tablepartmoment) {
        sprintf(name2, "%s-partmomentum",name);
        p=openfile(name2,"r");
        if (p) {
            m=0;
            while (m<graphdivisions) {
                fscanf(p,"%*lf %lf",tablepartmoment+m);
                m++;
            }
            fclose(p);
        }
    }
    if (type&PARTCURRENT && tablepartcurrent) {
        sprintf(name2, "%s-partcurrent",name);
        p=openfile(name2,"r");
        if (p) {
            m=0;
            while (m<graphdivisions) {
                fscanf(p,"%*lf %lf",tablepartcurrent+m);
                m++;
            }
            fclose(p);
        }
    }
}

double *inputspecdensity (char *name) {
    FILE *p=openfile(name, "r");
    if (!p) return NULL;
    double *t = malloc(sizeof(double)*rangedivisions);
    if (!t) return NULL;
    int n=0;
    while (n<rangedivisions) {
        fscanf(p, "%*lf %lf",t+n);
        n++;
    }
    return t;
}


// ========= Ouput domain wall solution for hard-rod problem ==============


void integratebottom(double x, double *table) {
    int n=0;
    double total=0;
    while (n<divisions) {
        table[n]=total*deltav;
        total+=preinitspectraldensity(x, getspeed(n));
        n++;
    }
}

void integratetop(double x, double *table) {
    int n=divisions-1;
    double total=0;
    while (n>=0) {
        table[n]=total*deltav;
        total+=preinitspectraldensity(x, getspeed(n));
        n--;
    }
}

void integratevbottom(double x, double *table) {
    int n=0;
    double total=0;
    while (n<divisions) {
        table[n]=total*deltav;
        total += getspeed(n) * preinitspectraldensity(x, getspeed(n));
        n++;
    }
}

void integratevtop(double x, double *table) {
    int n=divisions-1;
    double total=0;
    while (n>=0) {
        table[n]=total*deltav;
        total += getspeed(n) * preinitspectraldensity(x, getspeed(n));
        n--;
    }
}

void gtables(double *gtable, double *gptable, double *nutable) {
    double tb[divisions];
    double tt[divisions];
    double tvb[divisions];
    double tvt[divisions];
    integratebottom(0.5, tb);
    integratetop(-0.5, tt);
    integratevbottom(0.5, tvb);
    integratevtop(-0.5, tvt);
    double densm = 1 - rodlength(0, 0) * tt[0];
    double densp = 1 - rodlength(0, 0) * tb[divisions-1];
    int n=0;
    while (n<divisions) {
        gtable[n] = getspeed(n) - rodlength(0, 0) * ( tvt[n] / densm + tvb[n] / densp - getspeed(n) * (tt[n] / densm + tb[n] / densp) );
        gptable[n] = 1 + rodlength(0, 0) * (tt[n] / densm + tb[n] / densp);
        nutable[n] = tvt[n] / densm + tvb[n] / densp;
        n++;
    }
}

double inverse(double *table, double x) {
    int n=0;
    while (n<divisions) {
        if (table[n] > x) break;
        n++;
    }
    if (n==divisions) return getspeed(n-1);
    if (n>0) return getspeed(n-1) + (getspeed(n)-getspeed(n-1)) * (x - table[n-1]) / (table[n] - table[n-1]);
    return getspeed(n);
}

void outputsolution (char *name, double t) {
    if (t<timestep/2) return;
    int m;
    char name2[150];
    double gtable[divisions];
    double gptable[divisions];
    double nutable[divisions];
    double vstar;
    gtables(gtable, gptable, nutable);
    sprintf(name2, "%s-solution",name);
    FILE *p=openfile(name2,"w");
    if (p) {
        m=0;
        while (m<graphdivisions) {
            fprintf(p,"%lf %lf\n",getgraphposition(m), ( 1 - 1.0 / gptable[getspeedindex(inverse(gtable, getgraphposition(m)/t))] ) / rodlength(0, 0));
            m++;
        }
        fclose(p);
    }
    sprintf(name2, "%s-solutioncur",name);
    p=openfile(name2,"w");
    if (p) {
        m=0;
        while (m<graphdivisions) {
            vstar = inverse(gtable, getgraphposition(m)/t);
            fprintf(p,"%lf %lf\n",getgraphposition(m), nutable[getspeedindex(vstar)] / gptable[getspeedindex(vstar)]);
            m++;
        }
        fclose(p);
    }
}

//=========== output exact initial density =============-

void outputexactinitdensity(char *name) {
    char name2[150];
    int m,n;
    double val,total;
    sprintf(name2, "%s-initdensity",name);
    FILE *p=openfile(name2,"w");
    if (p) {
        n=0;
        total=0;
        while (n<rangedivisions) {
            m=0;
            val=0;
            while (m<rangedivisions) {
                val += initspectraldensity(getrangeposition(n), getrangespeed(m));
                m++;
            }
            val *= rangedeltav;
            fprintf(p, "%lf %lf\n",getrangeposition(n),val);
            total += val;
            n++;
        }
        printf("Total number of particles = %lf\n",total*rangedeltax);
        fclose(p);
    }
}


// ==================== Output data =======================



// save particle density into a table with filename "name-density.dat", and current into "name-current.dat", etc. samplnum takes values from 0 to anything, and says which sample we are looking at. takes average of all samples. type = 1: density, type = 2: current. type = 3: both.
// for the current: in the hard-rod problem, effective velocity is veff(v) = (v-au)/(1-a rho), where u = int dv v f(v) and rho = int dv f(v) (with f(v) number of particle per unit length and velocity). Current is int dv veff(v) f(v) = u. here we therefore just need to calculate u to have current.
// in LL model: veff(v) = v + int dw phi(v-w) (veff(v) - veff(w)) rho(w). current is again int dv veff(v) rho(v) = int dv v rho(v) by symmetry of phi. indeed this is equal to momentum density as it should. no need to solve integrale equations if we want to check just this.
// in order to calculate eos as function of v, we want to check rhopartial = int_v1^v2 dv f(v) integrated over x, as compared to int_v1^v2 dv veff(v)f(v) integrated over t. Then this is int_v1^v2 dv (v-au)/(1-a rho) f(v) = 1/(1-a rho) * (int_v1^v2 dv v f(v) - a u rhopartial). So we only need two things: upartial = int_v1^v2 dv v f(v) and rhopartial = int_v1^v2 dv f(v), in addition to rho and u. Needs to be sampled simultaneously, so one needs to decide v1 and v2 before sampling (otherwise we must record full fluid cells upon sampling....)
double calculateexactdensity(double x);
void output (char *name, int samplenum, int type, double t, int thestep) {
    //    printf("output %s, sample %d, time %lf\n",name,samplenum,totaltime);
    FILE *p;
    char name2[300];
    double x=graphposmin, val, valpart;
    double dx=graphdeltax;
    int m, k, n, npart, nvmin, nvmax, nspectral[graphspeeddivisions];
    double table[graphdivisions];
    double tablecorr[graphdivisions];
    double tablecurrent[graphdivisions];
    double tablepart[graphdivisions];
    double tablepartmoment[graphdivisions];
    double tablespecmin[graphdivisions];
    double tablespecmax[graphdivisions];
    double **tablespectral;
    if (type & SPECTRAL) {
        tablespectral = (double **)malloc(sizeof(double *)*graphdivisions);
        sprintf(name2, "%s-spectral",name);
        p=openfile(name2,"r");
        m=0;
        while (m<graphdivisions) {
            tablespectral[m] = (double *)malloc(sizeof(double)*graphspeeddivisions);
            k=0;
            while (k<graphspeeddivisions) {
                if (samplenum && p) fscanf(p,"%*lf %*lf %lf",&(tablespectral[m][k]));
                else tablespectral[m][k]=0;
                k++;
            }
            m++;
        }
        if (p) fclose(p);
    }
    struct rod *r=rods;
    
    m=0;
    while (m<graphdivisions) {
        table[m]=0;
        tablecorr[m]=0;
        tablecurrent[m]=0;
        tablepart[m]=0;
        tablepartmoment[m]=0;
        tablespecmin[m]=0;
        tablespecmax[m]=0;
        m++;
    }
    if (samplenum) input(name, type, table, tablecurrent, tablespecmin, tablespecmax, tablepart, tablepartmoment, NULL);
#ifdef OUTPUTCORRELATION
    if (samplenum && thestep) {
        sprintf(name2, "%s-corr",name);
        input(name2, type&DENSITY, tablecorr, NULL, NULL, NULL, NULL, NULL, NULL);
    }
#endif
    while (r && r->pos<x) r=r->next;
    m=0;
    while (m<graphdivisions) {
        val=0;
        n=0;
        valpart=0;
        npart=0;
        nvmin=0;
        nvmax=0;
        if (type & SPECTRAL) {
            k=0;
            while (k<graphspeeddivisions) {
                nspectral[k]=0;
                k++;
            }
        }
        while (r && r->pos<x+dx) {
            val += r->speed;
            n++;
            if (r->speed > partialspeedmin && r->speed < partialspeedmax) {
                valpart += r->speed;
                npart ++;
            }
            if (r->speed > partialspeedmin && r->speed < partialspeedmin + graphdeltav) {
                nvmin ++;
            }
            if (r->speed > partialspeedmax && r->speed < partialspeedmax + graphdeltav) {
                nvmax ++;
            }
            if (type & SPECTRAL) {
                k=getgraphspeedindex(r->speed);
                if (k>=0 && k<graphspeeddivisions) nspectral[k]++;
            }
            r=r->next;
        }
        table[m] = ( table[m]*samplenum + 1.0*n/dx ) / (samplenum+1);
        tablecurrent[m] = ( tablecurrent[m]*samplenum + val/dx ) / (samplenum+1);
        tablepart[m] = ( tablepart[m]*samplenum + npart/dx ) / (samplenum+1);
        tablepartmoment[m] = ( tablepartmoment[m]*samplenum + valpart/dx ) / (samplenum+1);
        tablespecmin[m] = ( tablespecmin[m]*samplenum + 1.0*nvmin/dx/graphdeltav ) / (samplenum+1);
        tablespecmax[m] = ( tablespecmax[m]*samplenum + 1.0*nvmax/dx/graphdeltav ) / (samplenum+1);
        if (type & SPECTRAL) {
            k=0;
            while (k<graphspeeddivisions) {
                tablespectral[m][k] = ( tablespectral[m][k]*samplenum + 1.0*nspectral[k]/dx/graphdeltav ) / (samplenum+1);
                // ICI- mettre /dx -> /dx/graphdeltav pour code correct dans la parenthese. ok done here.
                k++;
            }
        }
#ifdef OUTPUTCORRELATION
        if (thestep == 0 && x <= corrinitialposition && x+dx > corrinitialposition) {
            densityrawinit = 1.0*n/dx;
            if (samplenum == totalnumberofsamples-1) densityaverageinit = table[m];
            //            printf("\n rawinit: %lf\n",densityrawinit);
        }
        //        printf("%lf ",1.0*n/dx);
        if (thestep > 0) tablecorr[m] = (tablecorr[m]*samplenum + 1.0*n/dx*densityrawinit) / (samplenum+1);
#endif
        x+=dx;
        m++;
    }
    if (type&SPECTRAL) {
        sprintf(name2, "%s-spectral",name);
        p=openfile(name2,"w");
        if (p) {
            m=0;
            while (m<graphdivisions) {
                k=0;
                while (k<graphspeeddivisions) {
                    fprintf(p,"%lf %lf %lf\n",getgraphposition(m),getgraphspeed(k),tablespectral[m][k]);
                    k++;
                }
                fprintf(p,"\n");
                m++;
            }
            fclose(p);
        }
    }
    if (type&SPECTRALTWO) {
        sprintf(name2, "%s-spectralmin",name);
        p=openfile(name2,"w");
        if (p) {
            m=0;
            while (m<graphdivisions) {
                fprintf(p,"%lf %lf\n",getgraphposition(m),tablespecmin[m]);
                m++;
            }
            fclose(p);
        }
        sprintf(name2, "%s-spectralmax",name);
        p=openfile(name2,"w");
        if (p) {
            m=0;
            while (m<graphdivisions) {
                fprintf(p,"%lf %lf\n",getgraphposition(m),tablespecmax[m]);
                m++;
            }
            fclose(p);
        }
    }
    if (type&DENSITY) {
        sprintf(name2, "%s-density",name);
        p=openfile(name2,"w");
        if (p) {
            m=0;
            while (m<graphdivisions) {
                fprintf(p,"%lf %lf\n",getgraphposition(m),table[m]);
                m++;
            }
            fclose(p);
        }
#ifdef SOLUTIONBYITERATION
        sprintf(name2, "%s-soldensity",name);
        p=openfile(name2,"w");
        if (p) {
            m=0;
            while (m<graphdivisions) {
                fprintf(p,"%lf %lf\n",getgraphposition(m),calculateexactdensity(getgraphposition(m)));
                m++;
            }
            fclose(p);
        }
#endif
#ifdef OUTPUTCORRELATION
        if (thestep > 0) {
            sprintf(name2, "%s-corr-density",name);
            p=openfile(name2,"w");
            if (p) {
                m=0;
                while (m<graphdivisions) {
                    fprintf(p,"%lf %lf\n",getgraphposition(m),tablecorr[m]);
                    m++;
                }
                fclose(p);
            }        
            if (samplenum == totalnumberofsamples-1) {
                sprintf(name2, "%s-connected-density",name);
                p=openfile(name2,"w");
                if (p) {
                    m=0;
                    while (m<graphdivisions) {
                        fprintf(p,"%lf %lf\n",getgraphposition(m),tablecorr[m]-table[m]*densityaverageinit);
                        m++;
                    }
                    fclose(p);
                }                    
            }
        }
#endif
    }
    if (type&CURRENT) {
        sprintf(name2, "%s-current",name);
        p=openfile(name2,"w");
        if (p) {
            m=0;
            while (m<graphdivisions) {
                fprintf(p,"%lf %lf\n",getgraphposition(m),tablecurrent[m]);
                m++;
            }
            fclose(p);
        }
    }
    if (type&PARTDENSITY) {
        sprintf(name2, "%s-partdensity",name);
        p=openfile(name2,"w");
        if (p) {
            m=0;
            while (m<graphdivisions) {
                fprintf(p,"%lf %lf\n",getgraphposition(m),tablepart[m]);
                m++;
            }
            fclose(p);
        }
    }
    if (type&PARTCURRENT) {
        sprintf(name2, "%s-partmomentum",name);
        p=openfile(name2,"w");
        if (p) {
            m=0;
            while (m<graphdivisions) {
                fprintf(p,"%lf %lf\n",getgraphposition(m),tablepartmoment[m]);
                m++;
            }
            fclose(p);
        }
#ifdef SOFTRODS
        if (!samplenum) m=0;
        else{
            sprintf(name2, "%s-leftfluidcellsize",name);
            p=openfile(name2,"r");
            if (p) {
                fscanf(p, "%d",&m);
                fclose(p);
            }
            else m=0;
        }
        sprintf(name2, "%s-leftfluidcell",name);
        if (!samplenum) p=openfile(name2,"w");
        else p=openfile(name2,"a");
        if (p) {
            r = rods;
            while (r && r->pos<eulerleft) r=r->next;
            while (r && r->pos<eulerleft+dx) {
                fprintf(p,"%lf\n",r->speed);
                r=r->next;
                m++;
            }
            fclose(p);
        }
        sprintf(name2, "%s-leftfluidcellsize",name);
        p=openfile(name2,"w");
        if (p) {fprintf(p, "%d",m); fclose(p);}
        
        if (!samplenum) m=0;
        else{
            sprintf(name2, "%s-rightfluidcellsize",name);
            p=openfile(name2,"r");
            if (p) {
                fscanf(p, "%d",&m);
                fclose(p);
            }
            else m=0;
        }
        sprintf(name2, "%s-rightfluidcell",name);
        if (!samplenum) p=openfile(name2,"w");
        else p=openfile(name2,"a");
        if (p) {
            r = rods;
            while (r && r->pos<eulerright) r=r->next;
            while (r && r->pos<eulerright+dx) {
                fprintf(p,"%lf\n",r->speed);
                r=r->next;
                m++;
            }
            fclose(p);
        }
        sprintf(name2, "%s-rightfluidcellsize",name);
        p=openfile(name2,"w");
        if (p) {fprintf(p, "%d",m); fclose(p);}
#else
        sprintf(name2, "%s-partcurrent",name);
        p=openfile(name2,"w");
        if (p) {
            m=0;
            while (m<graphdivisions) {
                fprintf(p,"%lf %lf\n",getgraphposition(m),
                        (tablepartmoment[m] - rodlength(0,0)*tablecurrent[m]*tablepart[m]) /
                        (1-rodlength(0,0)*table[m]));
                m++;
            }
            fclose(p);
        }
#endif
    }
    if (type&DMSOLUTION) outputsolution(name, t);
    if (!thestep && type&EXACTINITIALDENSITY) outputexactinitdensity(name);
    if (type & SPECTRAL) {
        m=0;
        while (m<graphdivisions) {
            if (tablespectral[m]) free(tablespectral[m]);
            m++;
        }
        free (tablespectral);
    }
}


// ================ Recombination of multiple threads ==================

void recombinethreads(char *name) {
    
}


// ================ Check eos ==================

int check=1;

// table is the set of paricle velocities
double *calculateveff(double *table, int number) {
    if (!table) return NULL;
    double *veff = malloc(number*sizeof(double));
    double *vefftemp = malloc(number*sizeof(double));
    double u, w, err;
    int m,n,iter;
    if (veff) {
        m=0;
        while (m<number) {
            veff[m]=table[m];
            m++;
        }
        iter=0;
        while (iter<numberiterations) {
            m=0;
            while (m<number) {
                u=0;
                w=0;
                n=0;
                while (n<number) {
                    //                    u+=rodlength(table[m], table[n])*(veff[m]-veff[n]);
                    w+=rodlength(table[m], table[n]);
                    u+=rodlength(table[m], table[n])*veff[n];
                    n++;
                }
                u = u / (totalnumberofsamples*graphdeltax);
                w = w / (totalnumberofsamples*graphdeltax);
                //                vefftemp[m]=table[m] + u;
                //                vefftemp[m]=table[m] + w*veff[m] - u;
                vefftemp[m]=(table[m] - u)/(1-w);
                m++;
            }
            m=0;
            err=0;
            while (m<number) {
                if (fabs((veff[m]-vefftemp[m])/vefftemp[m])>err) err = fabs((veff[m]-vefftemp[m])/vefftemp[m]);
                veff[m] = vefftemp[m];
                m++;
            }
            if(check) printf("veff = %lf\n",veff[(int)(number/2)]);
            if (err<tolerance) break;
            if (iter == numberiterations-1 && err>tolerance) printf("imprecision in calculation veff: %lf\n",err);
            iter++;
        }
        check=0;
    }
    return veff;
}

void checkeos (char *name, double x1, double x2, double range, int type) {
    if (x1<graphposmin || x1>graphposmax || x2<x1 || x2>graphposmax) return;
    double table[graphdivisions];
    double *tableleft, *tableright, *tableveffl, *tableveffr;
    double densinit,densfin,curright,curleft,z,average;
    int m,n,leftpos,rightpos,timecount,totalcount,numberl, numberr;
    double forcemin=0, forcemax=0;
    char name2[300];
    m=0;
    while (m<graphdivisions) {
        table[m]=0;
        m++;
    }
    
#ifdef SOFTRODS
    double tableleftcurrent[numberofsteps];
    double tablerightcurrent[numberofsteps];
    if (type==PART) {
        n=0;
        FILE *pl, *pr;
        sprintf(name2, "%s-leftpartcurrent",name);
        pl = openfile(name2, "w");
        sprintf(name2, "%s-rightpartcurrent",name);
        pr = openfile(name2, "w");
        printf("Calculating effective velocities...\n");
        if (pl && pr) while (n<=numberofsteps) {
            sprintf(name2,"%s-%d",name,n);
            tableleft = inputleftfluidcell(name2,&numberl);
            tableright = inputrightfluidcell(name2,&numberr);
            tableveffl = calculateveff(tableleft,numberl);
            tableveffr = calculateveff(tableright,numberr);
#ifdef VEFF
            double min1,min2,max;
            int k,ct;
            FILE *pveff, *pv;
            sprintf(name2,"%s-leftveff-%d",name,n);
            pveff = openfile(name2, "w");
            sprintf(name2,"%s-leftv-%d",name,n);
            pv = openfile(name2, "w");
            min1 = tableleft[0];
            max = tableleft[0];
            k=0;
            m=0;
            while (m<numberl) {
                if (tableleft[m]<min1) {
                    min1 = tableleft[m];
                    k=m;
                }
                if (tableleft[m]>max) {
                    max = tableleft[m]+100;
                }
                m++;
            }
            fprintf(pveff, "%lf %lf\n",tableleft[k],tableveffl[k]);
            fprintf(pv, "%lf %lf\n",tableleft[k],tableleft[k]);
            ct=1;
            while (ct<numberl) {
                min2 = max;
                m=0;
                while (m<numberl) {
                    if (tableleft[m]<min2 && min1<tableleft[m]) {
                        min2 = tableleft[m];
                        k=m;
                    }
                    m++;
                }
                fprintf(pveff, "%lf %lf\n",tableleft[k],tableveffl[k]);
                fprintf(pv, "%lf %lf\n",tableleft[k],tableleft[k]);
                min1=min2;
                ct++;
            }
            fclose(pveff);
            fclose(pv);
            
#endif
            m=0;
            z=0;
            if (tableveffl) while (m<numberl) {
                if (tableleft[m]>partialspeedmin && tableleft[m]<partialspeedmax) z += tableveffl[m];
                m++;
            }
            z /= (totalnumberofsamples*graphdeltax);
            fprintf(pl, "%lf %lf\n",initialtime + n*timestep,z);
            tableleftcurrent[n]=z;
            m=0;
            z=0;
            if (tableveffr) while (m<numberr) {
                if (tableright[m]>partialspeedmin && tableright[m]<partialspeedmax) z += tableveffr[m];
                m++;
            }
            z /= (totalnumberofsamples*graphdeltax);
            fprintf(pr, "%lf %lf\n",initialtime + n*timestep,z);
            tablerightcurrent[n]=z;
            if (tableleft) free(tableleft);
            if (tableright) free(tableright);
            if (tableveffl) free(tableveffl);
            if (tableveffr) free(tableveffr);
            printf("%d\n",n);
            n++;
        }
        if (pl) fclose(pl);
        if (pr) fclose(pr);
    }
#endif
    
    
    leftpos = getgraphpositionindex(x1);
    rightpos = getgraphpositionindex(x2);
    totalcount = (int)(range/timestep+0.1);
    double tableeos[numberofsteps-totalcount];
    double tabletotalchange[numberofsteps-totalcount];
    timecount=0;
    average=0;
    while (timecount < numberofsteps - totalcount) {
        n=0;
        sprintf(name2,"%s-%d",name,n+timecount);
        if (type==FULL) input(name2, DENSITY, table, table, table, table, table, NULL, table);
        if (type==PART) input(name2, PARTDENSITY, table, table, table, table, table, NULL, table);
        densinit=0;
        m=leftpos;
        while (m<rightpos) {
            densinit+=table[m]*graphdeltax;
            m++;
        }
        curright=0;
        curleft=0;
        while (n<totalcount) {
            sprintf(name2,"%s-%d",name,n+timecount);
            if (type==FULL) input(name2, CURRENT, table, table, table, table, table, NULL, table);
#ifdef SOFTRODS
            if (type==PART) {
                table[leftpos] = tableleftcurrent[n+timecount];
                table[rightpos] = tablerightcurrent[n+timecount];
            }
#else
            if (type==PART) input(name2, PARTCURRENT, table, table, table, table, table, NULL, table);
#endif
            curleft += table[leftpos]*timestep;
            curright += table[rightpos]*timestep;
            n++;
        }
        sprintf(name2,"%s-%d",name,n+timecount);
        if (type==FULL) input(name2, DENSITY, table, table, table, table, table, NULL, table);
        if (type==PART) input(name2, PARTDENSITY, table, table, table, table, table, NULL, table);
        densfin=0;
        m=leftpos;
        while (m<rightpos) {
            densfin+=table[m]*graphdeltax;
            m++;
        }
        forcemin = 0;
        forcemax = 0;
#ifdef ACCELERATION
        if (type==PART) {
            n=0;
            while (n<totalcount) {
                sprintf(name2,"%s-%d",name,n+timecount);
                input(name2, SPECTRALTWO, NULL, NULL, table, NULL, NULL, NULL, NULL);
                m=leftpos;
                while (m<rightpos) {
                    forcemin += acceleration(getgraphposition(m)) * table[m] * graphdeltax * timestep;
                    m++;
                }
                input(name2, SPECTRALTWO, NULL, NULL, NULL, table, NULL, NULL, NULL);
                m=leftpos;
                while (m<rightpos) {
                    forcemax += acceleration(getgraphposition(m)) * table[m] * graphdeltax * timestep;
                    m++;
                }
                n++;
            }
        }
#endif
        printf("%lf, %lf\n",forcemin,forcemax);
        tableeos[timecount] = fabs((densinit-densfin+curleft-curright+forcemin-forcemax)/(densinit+densfin+fabs(curleft)+fabs(curright)));
        tabletotalchange[timecount] = densfin-densinit;
        average = (average*timecount + tableeos[timecount])/(timecount+1);
        timecount++;
    }
    sprintf(name2, "%s-euler",name);
    FILE *p=openfile(name2,"w");
    if (p) {
        m=0;
        while (m<numberofsteps-totalcount) {
            fprintf(p,"%lf %lf\n",initialtime+timestep*m,tableeos[m]);
            m++;
        }
        fprintf(p,"average: %lf\n",average);
        printf("average: %lf\n",average);
        fclose(p);
    }
    sprintf(name2, "%s-variation",name);
    p=openfile(name2,"w");
    if (p) {
        m=0;
        while (m<numberofsteps-totalcount) {
            fprintf(p,"%lf %lf\n",initialtime+timestep*m,tabletotalchange[m]);
            m++;
        }
        fclose(p);
    }
}


// ================== Test quasi-particle checks ============

void testparticleinit(void) {
#ifdef SOFTRODS
    double x=eulerleft;
    double dx=graphdeltax;
    int n;
    struct rod *r=rods;
    while (r && r->pos<x) r=r->next;
    n=0;
    while (r && r->pos<x+dx) {
        n++;
        r=r->next;
    }
    sizetabletestpart = n;
    tabletestpartid= NULL;
    tabletestpartpos=NULL;
    tabletestpartid = malloc(sizeof(struct rod *)*n+1);
    if (!tabletestpartid) return;
    tabletestpartpos = malloc(sizeof(double)*n+1);
    if (!tabletestpartpos) {free(tabletestpartid); return;}
    r=rods;
    while (r && r->pos<x) r=r->next;
    n=0;
    while (r && r->pos<x+dx) {
        tabletestpartid[n] = r;
        tabletestpartpos[n] = r->pos;
        n++;
        r=r->next;
    }    
#endif
}

void testparticleoutput(char *name) {
#ifdef SOFTRODS
    char name2[300];
    double tottime = numberofsteps*timestep;
    sprintf(name2,"%s-testpart",name);
    FILE *p = openfile(name2, "w");
    printf("saving test particle\n");
    double min1 = tabletestpartid[0]->speed;
    double max = tabletestpartid[0]->speed;
    double min2;
    int ct;
    int k=0;
    int m=0;
    while (m<sizetabletestpart) {
        if (tabletestpartid[m]->speed<min1) {
            min1 = tabletestpartid[m]->speed;
            k=m;
        }
        if (tabletestpartid[m]->speed>max) {
            max = tabletestpartid[m]->speed+100;
        }
        m++;
    }
    fprintf(p, "%lf %lf\n",tabletestpartid[k]->speed,(tabletestpartid[k]->pos-tabletestpartpos[k])/tottime);
    ct=1;
    while (ct<sizetabletestpart) {
        min2 = max;
        m=0;
        while (m<sizetabletestpart) {
            if (tabletestpartid[m]->speed<min2 && min1<tabletestpartid[m]->speed) {
                min2 = tabletestpartid[m]->speed;
                k=m;
            }
            m++;
        }
        fprintf(p, "%lf %lf\n",tabletestpartid[k]->speed,(tabletestpartid[k]->pos-tabletestpartpos[k])/totaltime);
        min1=min2;
        ct++;
    }
    fclose(p);
    if (tabletestpartid) free(tabletestpartid);
    if (tabletestpartpos) free(tabletestpartpos);
#endif
}


// ================== Drawing procedure ==================



void erase(char *name) {
    FILE *p=openfile(name,"w");
    if (p) fclose(p);
}

void drawconf(char *name) {
    int id;
    struct rod *r=rods;
    FILE *p=openfile(name,"a");
    if (!p) return;
    double x = graphposmin;
    double dx=graphdeltax;
    int m, n;
    while (r && r->pos<x) r=r->next;
    m=0;
    while (m<graphdivisions) {
        n=0;
        while (r && r->pos<x+dx) {id=r->id; r=r->next; n++;}
        //        if (n) fprintf(p,"X"); else fprintf(p," ");
        if (n) fprintf(p,"%d",id); else fprintf(p," ");
        x+=dx;
        m++;
    }
    fprintf(p,"\n");
    fclose(p);
}

void printconf(char *name) {
    struct rod *q=rods;
    FILE *p=openfile(name,"a");
    if (!p) return;
    fprintf(p,"%d\n",totalnumber);
    while (q) {
        fprintf(p,"%lf ",q->pos);
        q=q->next;
    }
    fprintf(p,"\n");
    fclose(p);
}


// ====================== Solution by iteration ======================

#ifdef SOLUTIONBYITERATION
void momentumprimedressed(double *twopirhos, double *occupation) {
    if (!twopirhos || !occupation) return;
    int iter,m,n;
    double val,err;
    double twopirhostemp[rangedivisions];
    n=0;
    while (n<rangedivisions) {
        twopirhos[n] = momentumprime(getrangespeed(n));
        n++;
    }
    iter=0;
    while (iter<numberiterations) {
        n=0;
        while (n<rangedivisions) {
            m=0;
            val=0;
            while (m<rangedivisions) {
                val += rodlength(getrangespeed(n), getrangespeed(m))*occupation[m]*twopirhos[m];
                m++;
            }
            twopirhostemp[n] = momentumprime(getrangespeed(n))*(1 - val/2/PI*rangedeltav);
            n++;
        }
        n=0;
        err=0;
        while (n<rangedivisions) {
            val = fabs((twopirhos[n]-twopirhostemp[n])/twopirhostemp[n]);
            if (val>err) err = val;
            twopirhos[n] = twopirhostemp[n];
            n++;
        }
        if (err<tolerance) break;
        if (iter == numberiterations-1 && err>tolerance) printf("imprecision in calculation of momentum prime dressed: %lf\n",err);
        iter++;
    }
}

void energyprimedressed(double *epd, double *occupation) {
    if (!epd || !occupation) return;
    int iter,m,n;
    double val,err;
    double epdtemp[rangedivisions];
    n=0;
    while (n<rangedivisions) {
        epd[n] = energyprime(getrangespeed(n));
        n++;
    }
    iter=0;
    while (iter<numberiterations) {
        n=0;
        while (n<rangedivisions) {
            m=0;
            val=0;
            while (m<rangedivisions) {
                val += rodlength(getrangespeed(n), getrangespeed(m))*occupation[m]*epd[m];
                m++;
            }
            epdtemp[n] = energyprime(getrangespeed(n)) - momentumprime(getrangespeed(n))*val/2/PI*rangedeltav;
            n++;
        }
        n=0;
        err=0;
        while (n<rangedivisions) {
            val = fabs((epd[n]-epdtemp[n])/epdtemp[n]);
            if (val>err) err = val;
            epd[n] = epdtemp[n];
            n++;
        }
        if (err<tolerance) break;
        if (iter == numberiterations-1 && err>tolerance) printf("imprecision in calculation of energy prime dressed: %lf\n",err);
        iter++;
    }
}

double calculateexactdensity(double x) {
    int n,m;
    double val;
    double mpd[rangedivisions];
    m=getrangepositionindex(x);
    momentumprimedressed(mpd, exactoccupation[m]);
    n=0;
    val=0;
    while (n<rangedivisions) {
        val += mpd[n]*exactoccupation[m][n];
        n++;
    }
    return val*rangedeltav/2/PI;
}

void sol_calculateoccupation(double t) {
    int n,m,k,iter;
    double *mpd[rangedivisions];
    double *mpdinit[rangedivisions];
    double *newoccupation[rangedivisions];
    double epdasympt[rangedivisions];
    double valright,valleft,valprec;
    // initialize tables
    n=0;
    while (n<rangedivisions) {
        mpd[n] = (double *)malloc(sizeof(double)*rangedivisions);
        mpdinit[n] = (double *)malloc(sizeof(double)*rangedivisions);
        newoccupation[n] = (double *)malloc(sizeof(double)*rangedivisions);
        if (!mpd[n] || ! mpdinit[n] || !newoccupation[n]) return;
        n++;
    }
    // initialize values
    n=0;
    while (n<rangedivisions) {
        momentumprimedressed(mpdinit[n], exactoccupation[n]);
        m=0;
        while (m<rangedivisions) {
            mpd[n][m] = mpdinit[n][m];
            newoccupation[n][m] = exactoccupation[n][m];
            m++;
        }
        n++;
    }
    energyprimedressed(epdasympt, exactoccupation[0]);
    iter=0;
    while (iter<totaliterations) {
        // calculate u(x,v) then n(x,v), for each x, v
        m=0; // m determines v
        while (m<rangedivisions) {
            n=0; // n determines x
            valright = -epdasympt[m]*t;
            k=0; // k determines u
            valleft=0;
            valprec=-1;
            while (n<rangedivisions) {
                while (valleft<valright && k<rangedivisions) {
                    valprec = valleft;
                    valleft += mpdinit[k][m]*rangedeltax;
                    k++;
                }
                if (k == rangedivisions || (k>0 && valleft - valright > valright - valprec)) newoccupation[n][m] = exactoccupation[k-1][m];
                else newoccupation[n][m] = exactoccupation[k][m];
                valright += mpd[n][m]*rangedeltax;
                n++;
            }
            m++;
        }
        n=0;
        while (n<rangedivisions) {
            momentumprimedressed(mpd[n], newoccupation[n]);
            n++;
        }
        iter++;
    }
    n=0;
    while (n<rangedivisions) {
        m=0;
        while (m<rangedivisions) {
            exactoccupation[n][m] = newoccupation[n][m];
            m++;
        }
        n++;
    }
    n=0;
    while (n<rangedivisions) {
        if (mpd[n]) free(mpd[n]);
        if (mpdinit[n]) free(mpdinit[n]);
        if (newoccupation[n]) free(newoccupation[n]);
        n++;
    }
}
#endif


// ====================== TBA stuff ===========================

#ifdef OUTPUTOCCUPATION
void calculateoccupation(double x) {
    double val;
    int n=0,m;
    if (!occupationfct) return;
    while (n<rangedivisions) {
        val=0;
        m=0;
        while (m<divisions) {
            val += rodlength(getrangespeed(n), getspeed(m))*preinitspectraldensity(x, getspeed(m));
            m++;
        }
        occupationfct[n] = 1 - momentumprime(getrangespeed(n))*val*deltav;
        n++;
    }
    n=0;
    while (n<rangedivisions) {
        occupationfct[n] = 2*PI*preinitspectraldensity(x, getrangespeed(n))/occupationfct[n];
        //        if (n>2900 && n<3100) printf("%lf %lf\n",getrangespeed(n),occupationfct[n]);
        n++;
    }
}
#endif

#ifdef FROMINITIALGGE
void calculateinitialspectraldensityzeroT(double *sd, int k, char *name) {
    int n,m,iter;
    double ep[rangedivisions];
    double twopirhos[rangedivisions];
    double twopirhostemp[rangedivisions];
    double val,err;
    double x = getrangeposition(k);
    
    // retrieve fermi energies from file if asked to
#ifdef RETRIEVEFERMIPOINTS
    FILE *retrievepointer=openfile(name,"r");
    if (name && retrievepointer) {
        int j,sign;
        double point=0;
        n=rangedivisions;
        while (n--) {
            fscanf(retrievepointer,"%d %d",&j,&m);
            if (j==k) break;
            while (m--) {
                fscanf(retrievepointer, "%*lf");
            }
        }
        if (!n) m=0;
        printf("Retrieving x=%lf\n",x);
        n=0;
        if (m) fscanf(retrievepointer, "%lf",&point);
        sign=1;
        while (n<rangedivisions) {
            ep[n] = sign;
            if (m && n<rangedivisions-1 && getrangespeed(n)<point && getrangespeed(n+1)>point){
                sign=-sign;
                m--;
                if (m) fscanf(retrievepointer, "%lf",&point);
            }
            n++;
        }
        fclose(retrievepointer);
    }
    else {
        n=0;
        while (n<rangedivisions) {
            ep[n] = 1;
            n++;
        }        
    }
#else
    FILE *p=NULL;
    double eptemp[rangedivisions];
    char temporary[200]="";
    {
        // calculate fermi energies
        n=0;
        while (n<rangedivisions) {
            ep[n] = initialgge(x, getrangespeed(n));
            n++;
        }
        iter=0;
        while (iter<numberiterations) {
            n=0;
            while (n<rangedivisions) {
                m=0;
                val=0;
                while (m<rangedivisions) {
                    if (ep[m]<0) val += rodlength(getrangespeed(n), getrangespeed(m))*ep[m];
                    m++;
                }
                eptemp[n] = initialgge(x, getrangespeed(n)) - val/2/PI*rangedeltav*momentumprime(getrangespeed(n));
                n++;
            }
            n=0;
            err=0;
            while (n<rangedivisions) {
                val = fabs((exp(ep[n])-exp(eptemp[n]))/exp(eptemp[n]));
                if (val>err) err = val;
                ep[n] = eptemp[n];
                n++;
            }
            if (err<tolerance)
                break;
            if (iter == numberiterations-1 && err>tolerance) printf("imprecision in calculation of epsilon: %lf\n",err);
            iter++;
        }
        if (name) {
            if (k>0) p = openfile(name,"a");
            else p = openfile(name, "w");
        } else p=NULL;
        printf("%lf: ",x);
        if (p) fprintf(p, "%d ",k);
        n=0;
        m=0;
        while (n<rangedivisions-1) {
            if (ep[n]*ep[n+1]<0) {
                printf("%lf ",(getrangespeed(n)+getrangespeed(n+1))/2);
                sprintf(temporary, "%s%lf ",temporary,(getrangespeed(n)+getrangespeed(n+1))/2);
                m++;
            }
            n++;
        }
        printf("\n");
        if (p) fprintf(p, "%d %s\n",m,temporary);
        fclose(p);
    }
#endif
    
    // get state density. use the function 2 pi rho_s = (p')^dr.
    n=0;
    while (n<rangedivisions) {
        twopirhos[n] = momentumprime(getrangespeed(n));
        n++;
    }
    iter=0;
    while (iter<numberiterations) {
        n=0;
        while (n<rangedivisions) {
            m=0;
            val=0;
            while (m<rangedivisions) {
                if (ep[m]<0) val += rodlength(getrangespeed(n), getrangespeed(m))*twopirhos[m];
                m++;
            }
            twopirhostemp[n] = momentumprime(getrangespeed(n))*(1 - val/2/PI*rangedeltav);
            n++;
        }
        n=0;
        err=0;
        while (n<rangedivisions) {
            val = fabs((twopirhos[n]-twopirhostemp[n])/twopirhostemp[n]);
            if (val>err) err = val;
            twopirhos[n] = twopirhostemp[n];
            n++;
        }
        if (err<tolerance) break;
        if (iter == numberiterations-1 && err>tolerance) printf("imprecision in calculation of 2 pi rho_s: %lf, x=%lf\n",err,x);
        iter++;
    }
    //    printf("%lf \n",twopirhos[getrangepositionindex(0)]);
    
    // calculate spectral density
    n=0;
    while (n<rangedivisions) {
        if (ep[n]<0) sd[n] = twopirhos[n]/2/PI;
        else sd[n] = 0;
#ifdef SOLUTIONBYITERATION
        if (ep[n]<0) exactoccupation[getrangepositionindex(x)][n] = 1;
        else exactoccupation[getrangepositionindex(x)][n] = 0;
#endif
        n++;
    }
}

void calculateinitialspectraldensity(double *sd, double x) {
    int n,m,iter;
    double L[rangedivisions];
    double occupation[rangedivisions];
    double twopirhos[rangedivisions];
    double twopirhostemp[rangedivisions];
    double val,err,temp;
    double eptemp[rangedivisions];
    
    // calculate initial L function
    n=0;
    while (n<rangedivisions) {
        L[n] = log(1+exp(-initialgge(x, getrangespeed(n))));
        n++;
    }
    iter=0;
    while (iter<numberiterations) {
        n=0;
        while (n<rangedivisions) {
            m=0;
            val=0;
            while (m<rangedivisions) {
                val += rodlength(getrangespeed(n), getrangespeed(m))*L[m];
                m++;
            }
            eptemp[n] = initialgge(x, getrangespeed(n)) + val/2/PI*rangedeltav*momentumprime(getrangespeed(n));
            n++;
        }
        n=0;
        err=0;
        while (n<rangedivisions) {
            temp = log(1+exp(-eptemp[n]));
            val = fabs((L[n]-temp)/temp);
            if (val>err) err = val;
            L[n] = temp;
            n++;
        }
        if (err<tolerance)
            break;
        if (iter == numberiterations-1 && err>tolerance) printf("imprecision in calculation of epsilon: %lf\n",err);
        iter++;
    }
    printf("%lf\n",x);
    
    // get state density. use the function 2 pi rho_s = (p')^dr.
    n=0;
    while (n<rangedivisions) {
        twopirhos[n] = momentumprime(getrangespeed(n));
        occupation[n] = 1.0-1.0/exp(L[n]);
        n++;
    }
    iter=0;
    while (iter<numberiterations) {
        n=0;
        while (n<rangedivisions) {
            m=0;
            val=0;
            while (m<rangedivisions) {
                val += rodlength(getrangespeed(n), getrangespeed(m))*occupation[m]*twopirhos[m];
                m++;
            }
            twopirhostemp[n] = momentumprime(getrangespeed(n))*(1 - val/2/PI*rangedeltav);
            n++;
        }
        n=0;
        err=0;
        while (n<rangedivisions) {
            val = fabs((twopirhos[n]-twopirhostemp[n])/twopirhostemp[n]);
            if (val>err) err = val;
            twopirhos[n] = twopirhostemp[n];
            n++;
        }
        if (err<tolerance) break;
        if (iter == numberiterations-1 && err>tolerance) printf("imprecision in calculation of 2 pi rho_s: %lf, x=%lf\n",err,x);
        iter++;
    }
    
    // calculate spectral density, and record occupation function if necessary
    n=0;
    while (n<rangedivisions) {
        sd[n] = occupation[n]*twopirhos[n]/2/PI;
#ifdef SOLUTIONBYITERATION
        exactoccupation[getrangepositionindex(x)][n] = occupation[n];
#endif
        n++;
    }
}
#endif


// ====================== Time averaging stuff ================

double calculatermsspectral(char *name1, char *name2) {
    char nameloc[200];
    int m,k;
    FILE *p1, *p2;
    double v1,v2,rms,rmssum;
    sprintf(nameloc, "%s-spectral",name1);
    p1=openfile(nameloc,"r");
    sprintf(nameloc, "%s-spectral",name2);
    p2=openfile(nameloc,"r");
    if (!p1 || !p2) return 0;
    m=0;
    rms=0;
    rmssum=0;
    while (m<graphdivisions) {
        k=0;
        while (k<graphspeeddivisions) {
            fscanf(p1,"%*lf %*lf %lf",&v1);
            fscanf(p2,"%*lf %*lf %lf",&v2);
            rms+=(v1-v2)*(v1-v2);
            rmssum += (v1+v2)*(v1+v2);
            k++;
        }
        m++;
    }
    fclose(p1);
    fclose(p2);
    return sqrt(rms/rmssum);
}

// ====================== Retrieve / save rod distribution, for branching ================

#ifdef BRANCHING
void retrieveroddistribution (void) {
    struct rod *r=NULL;
    while (rods) {
        r=rods->next;
#ifdef SOFTRODS
        free(rods->tableactiverods);
#endif
        free(rods);
        rods = r;
    }
    rods = savedrods;
    totaltime = savedtotaltime;
}

void saveroddistribution (void) {
    struct rod *r = rods;
    struct rod *sr = NULL;
    struct rod *prev = NULL;
    int n,m;
    m=0;
    while (m<totalnumber) {
        sr = malloc(sizeof(struct rod));
        sr->pos = r->pos;
        sr->speed = r->speed;
#ifdef SOFTRODS
        sr->id = r->id;
        sr->tableactiverods = (int *)malloc(sizeof(int)*totalnumber);
        n=0;
        while (n<totalnumber) {
            sr->tableactiverods[n] = r->tableactiverods[n];
            n++;
        }
        sr->prev = prev;
#endif
        if (prev) prev->next = sr;
        sr->next = NULL;
        prev = sr;
        if (m==0) savedrods = sr;
        r=r->next;
        m++;
    }
    savedtotaltime = totaltime;
}

#endif

// ====================== Recombine two equally-sampled strands of spectral data ===============================

/*
void recombine (char *name, char *name1, char *name2) {
    double **tablespectral;
    double val;
    char nameth[100];
    FILE *p;
    int m,k;
    
    tablespectral = (double **)malloc(sizeof(double *)*graphdivisions);
    sprintf(nameth, "%s-spectral",name1);
    p=openfile(nameth,"r");
    if (!p) {printf("aaa1");return;}
    m=0;
    while (m<graphdivisions) {
        tablespectral[m] = (double *)malloc(sizeof(double)*graphspeeddivisions);
        k=0;
        while (k<graphspeeddivisions) {
            fscanf(p,"%*lf %*lf %lf",&(tablespectral[m][k]));
            k++;
        }
        m++;
    }
    if (p) fclose(p);
    sprintf(nameth, "%s-spectral",name2);
    p=openfile2(nameth,"r");
    if (!p) {printf("aaa2");return;}
    m=0;
    while (m<graphdivisions) {
        k=0;
        while (k<graphspeeddivisions) {
            fscanf(p,"%*lf %*lf %lf",&val);
            tablespectral[m][k] = (tablespectral[m][k] + val)/2;
            k++;
        }
        m++;
    }
    if (p) fclose(p);
    sprintf(nameth, "%s-spectral",name);
    p=openfile(nameth,"w");
    if (!p) {printf("aaa3");return;}
    m=0;
    while (m<graphdivisions) {
        k=0;
        while (k<graphspeeddivisions) {
            fprintf(p,"%lf %lf %lf\n",getgraphposition(m),getgraphspeed(k),tablespectral[m][k]);
            k++;
        }
        m++;
    }
    if (p) fclose(p);
    m=0;
    while (m<graphdivisions) {
        if (tablespectral[m]) free(tablespectral[m]);
        m++;
    }
    free (tablespectral);
}
*/

// ====================== Calculating the GHD conserved function ===================

void smoothout (char *name) {
    double **tablespectral, **tablespectralsm,val;
    char nameth[300];
    FILE *p;
    int m,k,l,n;
    tablespectral = (double **)malloc(sizeof(double *)*graphdivisions);
    tablespectralsm = (double **)malloc(sizeof(double *)*graphdivisions);
    sprintf(nameth, "%s-spectral",name);
    p=openfile(nameth,"r");
    if (!p) {printf("aaa");return;}
    m=0;
    while (m<graphdivisions) {
        tablespectral[m] = (double *)malloc(sizeof(double)*graphspeeddivisions);
        tablespectralsm[m] = (double *)malloc(sizeof(double)*graphspeeddivisions);
        k=0;
        while (k<graphspeeddivisions) {
            fscanf(p,"%*lf %*lf %lf",&(tablespectral[m][k]));
            k++;
        }
        m++;
    }
    if (p) fclose(p);
    m=3;
    while (m<graphdivisions-3) {
        k=3;
        while (k<graphspeeddivisions-3) {
            l=-3;
            val=0;
            while (l<=3) {
                n=-3;
                while (n<=3) {
                    val+=tablespectral[m+l][k+n];
                    n++;
                }
                l++;
            }
            //            tablespectral[m][k] = (tablespectral[m][k] + tablespectral[m-1][k]+tablespectral[m][k-1]+tablespectral[m-1][k-1])/4;
            /*            tablespectralsm[m][k] = (tablespectral[m][k] + tablespectral[m-1][k] + tablespectral[m+1][k]+
             tablespectral[m][k-1]+tablespectral[m-1][k-1] + tablespectral[m+1][k-1]+
             tablespectral[m][k+1]+tablespectral[m-1][k+1] + tablespectral[m+1][k+1])/9;*/
            tablespectralsm[m][k] = val/49;
            k++;
        }
        m++;
    }
    sprintf(nameth, "%s-spectralsmooth7",name);
    p=openfile(nameth,"w");
    if (!p) {printf("aaa");return;}
    m=0;
    while (m<graphdivisions) {
        k=0;
        while (k<graphspeeddivisions) {
            fprintf(p,"%lf %lf %lf\n",getgraphposition(m),getgraphspeed(k),tablespectralsm[m][k]);
            k++;
        }
        m++;
    }
    if (p) fclose(p);
    m=0;
    while (m<graphdivisions) {
        if (tablespectral[m]) free(tablespectral[m]);
        if (tablespectralsm[m]) free(tablespectralsm[m]);
        m++;
    }
    free (tablespectral);
}

void compare (char *name1, char *name2) {
    double **tablespectral;
    double val,val2,val3;
    char nameth[100];
    FILE *p;
    int m,k;
    
    tablespectral = (double **)malloc(sizeof(double *)*graphdivisions);
    sprintf(nameth, "%s-spectralsmooth7",name1);
    p=openfile(nameth,"r");
    if (!p) {printf("aaa");return;}
    m=0;
    while (m<graphdivisions) {
        tablespectral[m] = (double *)malloc(sizeof(double)*graphspeeddivisions);
        k=0;
        while (k<graphspeeddivisions) {
            fscanf(p,"%*lf %*lf %lf",&(tablespectral[m][k]));
            // ICI- this next line is to correct the unnormalized data; to be taken away for correct code. ok taken away.
            //            tablespectral[m][k] = tablespectral[m][k]/graphdeltav;
            k++;
        }
        m++;
    }
    if (p) fclose(p);
    val2=0;
    val3=0;
    sprintf(nameth, "%s-spectralsmooth7",name2);
    p=openfile2(nameth,"r");
    if (!p) {printf("aaa");return;}
    m=0;
    while (m<graphdivisions) {
        k=0;
        while (k<graphspeeddivisions) {
            fscanf(p,"%*lf %*lf %lf",&val);
            val2 += fabs(val - tablespectral[m][k]);
            val3+=tablespectral[m][k];
            k++;
        }
        m++;
    }
    if (p) fclose(p);
    printf("%lf\n",val2/val3);
    m=0;
    while (m<graphdivisions) {
        if (tablespectral[m]) free(tablespectral[m]);
        m++;
    }
    free (tablespectral);
}

void calculateconservedqty(char *name)
{
    double **tablespectral, ntable[graphspeeddivisions], occfct[graphspeeddivisions];
    double val,val2,val3,val4;
    char name2[100];
    FILE *p;
    int n,m,k;
    
    tablespectral = (double **)malloc(sizeof(double *)*graphdivisions);
    sprintf(name2, "%s-spectral",name);
    p=openfile(name2,"r");
    m=0;
    while (m<graphdivisions) {
        tablespectral[m] = (double *)malloc(sizeof(double)*graphspeeddivisions);
        k=0;
        while (k<graphspeeddivisions) {
            fscanf(p,"%*lf %*lf %lf",&(tablespectral[m][k]));
            // ICI- this next line is to correct the unnormalized data; to be taken away for correct code. ok taken away
            //                        tablespectral[m][k] = tablespectral[m][k]/graphdeltav;
            k++;
        }
        m++;
    }
    if (p) fclose(p);
    
    n=0;
    while (n<graphspeeddivisions) {
        ntable[n]=0;
        n++;
    }
    
    k=0;
    while (k<graphdivisions) {
        n=0;
        while (n<graphspeeddivisions) {
            val=0;
            m=0;
            while (m<graphspeeddivisions) {
                val += rodlength(getgraphspeed(n), getgraphspeed(m))*tablespectral[k][m];
                m++;
            }
            occfct[n] = 1 - momentumprime(getgraphspeed(n))*val*graphdeltav;
            n++;
        }
        n=0;
        while (n<graphspeeddivisions) {
            occfct[n] = 2*PI*tablespectral[k][n]/occfct[n];
            ntable[getoccupationtableindex(occfct[n])]+=1.0/graphdeltan*graphdeltav*graphdeltax*tablespectral[k][n];
            n++;
        }
        k++;
    }
    sprintf(name2, "%s-conserved",name);
    p=openfile(name2,"w");
    n=0;
    while (n<graphspeeddivisions) {
        fprintf(p, "%lf %lf\n",getoccupationtablevalue(n),ntable[n]);
        n++;
    }
    fclose(p);
    m=0;
    val=0;
    val2=0;
    while (m<graphspeeddivisions) {
        val+=(-log(getoccupationtablevalue(m)))*ntable[m]*graphdeltan;
        val2+=-( log(getoccupationtablevalue(m))+
                (1-getoccupationtablevalue(m))/getoccupationtablevalue(m) * log(1-getoccupationtablevalue(m)))*ntable[m]*graphdeltan;
        m++;
    }
    val3=0;
    val4=0;
    k=0;
    while (k<graphdivisions) {
        m=1;
        while (m<graphspeeddivisions) {
            val3 += fabs(tablespectral[k][m] - tablespectral[k][m-1])/2;
            val4 += tablespectral[k][m];
            m++;
        }
        k++;
    }
    val3=val3/val4;
    printf("entropy, YY entropy, noise, checksum:\n");
    printf("%lf, %lf, %lf, %lf\n",val,val2,val3,val4*graphdeltav*graphdeltax);
    m=0;
    while (m<graphdivisions) {
        if (tablespectral[m]) free(tablespectral[m]);
        m++;
    }
    free (tablespectral);
}

// ====================== Initialization etc. ==================

double meandistance(void) {
    double d=0;
    struct rod *r=rods;
    while (r) {
        if (r->next) d += r->next->pos - r->pos;
        r=r->next;
    }
    return d / (totalnumber-1);
}

int initialization (void) {
    if (divisions <= 1) return 0;
    if (initposmax<=initposmin) return 0;
    if (initspeedmax<=initspeedmin) return 0;
    deltax = (initposmax - initposmin)/(divisions);
    deltav = (initspeedmax - initspeedmin)/(divisions);
    rangedeltax = (rangeposmax - rangeposmin)/(rangedivisions);
    rangedeltav = (rangespeedmax - rangespeedmin)/(rangedivisions);
    graphdeltax = (graphposmax - graphposmin)/graphdivisions;
    graphdeltav = (graphspeedmax - graphspeedmin)/(graphspeeddivisions);
    graphdeltan = (1.0 - 0.0)/(graphspeeddivisions);
    smalldisplacement = (initposmax-initposmin)/divisions/sqrt(divisions);
#ifdef CONSERVEDQTYONLY
    return 1;
#endif
#ifdef RECOMBINEONLY
    return 1;
#endif
#ifdef OUTPUTOCCUPATION
    occupationfct = malloc(sizeof(double)*rangedivisions);
    if (!occupationfct) return 0;
#endif
#ifdef SOLUTIONBYITERATION
    exactoccupation = (double **)malloc(sizeof(double *)*rangedivisions);
    if (!exactoccupation) return 0;
    n=0;
    while (n<rangedivisions) {
        exactoccupation[n] = (double *)malloc(sizeof(double)*rangedivisions);
        if (!exactoccupation[n]) return 0;
        n++;
    }
#endif
#ifdef FROMINITIALGGE
    int n;
    thetablesd = malloc(sizeof(double *)*rangedivisions);
    if (!thetablesd) return 0;
    char name[200];
    sprintf(name, "%s-fermipoints",thename);
    n=0;
    while (n<rangedivisions) {
        thetablesd[n] = malloc(sizeof(double)*rangedivisions);
        if (!thetablesd[n]) return 0;
#ifdef ZEROTEMPERATURE
        calculateinitialspectraldensityzeroT(thetablesd[n],n,name);
#else
        calculateinitialspectraldensity(thetablesd[n],getrangeposition(n));
#endif
        n++;
    }
#endif
#ifdef OUTPUTCORRELATION
    tableinitial = (double *)malloc(sizeof(double)*graphdivisions);
    if (!tableinitial) return 0;
#endif
#ifdef PERFORMANCE
    int k;
    perfodeltax = (perfoposmax - perfoposmin)/(perfodivisions);
    perfodeltav = (perfospeedmax - perfospeedmin)/(perfodivisions);
#ifdef ACCELERATION
    k=0;
    accelerationtable = (double *)malloc(sizeof(double)*perfodivisions);
    if (!accelerationtable) return 0;
    while (k<perfodivisions) {
        accelerationtable[k] = acceleration(getperfoposition(k));
        k++;
    }
#endif
#ifdef PERFORODTABLE
#ifndef PERFORODTABLEDIFF
    int l;
    rodlengthtable = (double **)malloc(sizeof(double *)*perfodivisions);
    if (!rodlengthtable) return 0;
    k=0;
    while (k<perfodivisions) {
        rodlengthtable[k] = (double *)malloc(sizeof(double)*perfodivisions);
        if (!rodlengthtable[k]) return 0;
        l=0;
        while (l<perfodivisions) {
            l++;
            rodlengthtable[k][l] = rodlength(getperfospeed(k), getperfospeed(l));
        }
        k++;
    }
#else
    k=0;
    rodlengthtablediff = (double *)malloc(sizeof(double)*perfodivisions);
    if (!rodlengthtablediff) return 0;
    while (k<perfodivisions) {
        rodlengthtablediff[k] = rodlength(getperfospeed(k),0);
        k++;
    }
#endif
#endif
#endif
    totaltime = 0;
    return 1;
}


// ========= output tex file =============

#ifdef TEXFILEONLY
void outputtexfile(void) {
    char name[300];
    FILE *p;
    int n;
#ifdef CONSERVEDQTYONLY
    sprintf(name, "%s%s-plotcons.tex",pathofoutput,thename);
#else
    sprintf(name, "%s%s-plot.tex",pathofoutput,thename);
#endif
    p=fopen(name,"w");
    if (!p) return;
    fprintf(p, "\\documentclass[a4paper,11pt]{article} \\pdfoutput=1 \\setlength{\\textwidth}{160mm} \\setlength{\\textheight}{230mm} \\setlength{\\oddsidemargin}{-1cm} \\setlength{\\evensidemargin}{-1cm} \\usepackage{pgfplots} \\pgfplotsset{compat=1.3}  \\usepackage{amsmath} \\usepackage{graphicx} \\usepackage{amsfonts} \\usepackage{amssymb} \\begin{document}\n");
    n=0;
    while (n<numberofsteps+1) {
        fprintf(p, additionallineinit);
#ifdef CONSERVEDQTYONLY
        fprintf(p, "\\begin{tikzpicture}[scale=%s] \\begin{axis}[%s]\n",scalefortexoutput,linefortexoutputdensity);
        fprintf(p, "\\addplot[blue] table {%s%s-%d-conserved.dat};\n",pathofoutput,thename,n);
        fprintf(p, "\\end{axis} \\end{tikzpicture}\n");
#endif
        if (outputtype & (DENSITY | EXACTINITIALDENSITY) ) {
            fprintf(p, "\\begin{tikzpicture}[scale=%s] \\begin{axis}[%s]\n",scalefortexoutput,linefortexoutputdensity);
            if (outputtype & DENSITY) {
#ifdef BRANCHING
                fprintf(p, "\\addplot[blue] table {%s%s_%d-%d-density.dat};\n",pathofoutput,thename,branchofoutput,n);
#else
                fprintf(p, "\\addplot[blue] table {%s%s-%d-density.dat};\n",pathofoutput,thename,n);
#endif
#ifdef SOLUTIONBYITERATION
                fprintf(p, "\\addplot[green] table {%s%s-%d-soldensity.dat};\n",pathofoutput,thename,n);
#endif
            }
            if (outputtype & EXACTINITIALDENSITY)
                fprintf(p, "\\addplot[red] table {%s%s-0-initdensity.dat};\n",pathofoutput,thename);
            fprintf(p, "\\end{axis} \\end{tikzpicture}\n");
            fprintf(p, additionallinedensity);
        }
#ifdef OUTPUTCORRELATION
        if (outputtype & DENSITY && n>0) {
            fprintf(p, "\\begin{tikzpicture}[scale=%s] \\begin{axis}[%s]\n",scalefortexoutput,linefortexoutputcorrelation);
            fprintf(p, "\\addplot[red] table {%s%s-%d-connected-density.dat};\n",pathofoutput,thename,n);
            fprintf(p, "\\end{axis} \\end{tikzpicture}\n");
            fprintf(p, additionallinecorrelation);
        }
#endif
        if (outputtype & CURRENT) {
            fprintf(p, "\\begin{tikzpicture}[scale=%s] \\begin{axis}[%s]\n",scalefortexoutput,linefortexoutputcurrent);
            fprintf(p, "\\addplot[red] table {%s%s-%d-current.dat};\n",pathofoutput,thename,n);
            fprintf(p, "\\end{axis} \\end{tikzpicture}\n");
            fprintf(p, additionallinecurrent);
        }
        if (outputtype & SPECTRAL) {
            fprintf(p, "\\begin{tikzpicture}[scale=%s] \\begin{axis}[colorbar,%s]\n",scalefortexoutput,linefortexoutputspectral);
            fprintf(p, "\\addplot[only marks, scatter, point meta=explicit, mark=square*, mark size=%s] file {%s%s-%d-spectral.dat};\n",
                    pointsizescatterplot,pathofoutput,thename,n);
            fprintf(p, "\\end{axis} \\end{tikzpicture}\n");
            fprintf(p, additionallinespectral);
        }
        fprintf(p, additionallinefin);
        
        fprintf(p, "\\newpage\n\n");
        n+= texoutputskip;
    }
    fprintf(p, "\\end{document}\n\n");
}
#endif


// ========= Main procedure ==============



int main (int argc, const char * argv[])
{
    /* Analysis of scaling of time with respect to quality of microscopic, with performance turned on, and with new collision-tree procedure.
     In the case of harmonic potential, with delta t = 0.005, 5 samples of 50 steps each of time 1.93, we find very precisely
     time = 16 + 4*lengthscale^{1.584963}
     from the time data (sec) 20->28->52->124 at lengthscales 1->2->4->8
        (this is time data of 5 samples divided by 2^n to keep same noise level)
     Scaling exponent is probably more universal: 1.584963.
     */
    /*
    double cc=0;
    while (cc<20) {
        printf("%lf: %lf, %lf, %lf\n",cc,log((28-cc)/(20-cc))/log(2), log((52-cc)/(28-cc))/log(2),log((124-cc)/(52-cc))/log(2));
        cc+=1;
    }
    return 0;
     */
    
    if (!para_start(numberofthreads, &argc, argv)) return -1;

    char name[300];
    int n,c,m;
    int count;
    int headertrue=FALSE;
#ifdef RMSSPECTRALCHANGE
    if (_para_currentthread>0) return 0;
    int j,flagsampling;
    char nameoth[300];
#endif
#ifdef BRANCHING
    int thebranch;
#endif

#ifdef TEXFILEONLY
    if (_para_currentthread==0) outputtexfile();
    return 0;
#endif
    
    printf("Initialization...\n");
    if (!initialization()) {
        printf("Problem initialization\n");
        return -1;
    }
    printf("OK\n");
    
#ifdef CONSERVEDQTYONLY
    if (_para_currentthread==0) {
        //    char nameoth[300];
        n=0;
        while (n<numberofsteps + 1) {
            sprintf(name, "%s-%d",thename,n);
            //        smoothout(name);
            calculateconservedqty(name);
            //        sprintf(nameoth, "%s-%d",thename2,n);
            //        compare(name, nameoth);
            n++;
        }
    }
    return 0;
#endif

    /* // no longer used as we have parallel engine
#ifdef RECOMBINEONLY
    char name1[300];
    char name2[300];
    n=0;
    while (n<numberofsteps + 1) {
        sprintf(name, "%s-%d",thename,n);
        sprintf(name1, "%s-%d",thename1,n);
        sprintf(name2, "%s-%d",thename2,n);
        recombine(name, name1, name2);
        n++;
    }
    return 0;
#endif
     */
    
    FILE *p;
    FILE *header;
    if (_para_currentthread == 0) {
        p = fopen(fullnameofthisfile, "r");
        if (!p) printf("Problem opening source file in order to create header\n");
        else {
#ifdef LOOP
            sprintf(name, "%s%s-header-run.txt",pathofoutput,thename);
#else
            sprintf(name, "%s%s-header.txt",pathofoutput,thename);
#endif
            header = fopen(name, "w");
            if (!header) {
                printf("Problem creating hearder file\n");
                fclose(p);
            }
        }
        if (p && header) headertrue=1;
        if (headertrue) {
            do {
                if (fgetc(p)=='*') if (!feof(p) && fgetc(p)=='*') if (!feof(p) && fgetc(p)=='*') break;
            } while (!feof(p));
            n=0;
            if (!feof(p)) do {
                c = fgetc(p);
                if (c=='*') n++;
                else n=0;
                fputc(c, header);
                if (n==3) break;
            } while (!feof(p));
            fclose(p);
        }
    } else headertrue = 0;
    
    /*
     // This is to read data for left and right spectral density; only needed for domain wall initial condition with out-sourced initial spectral density
     sprintf(name, "%s-lsd",thename);
     leftsd = inputspecdensity(name);
     sprintf(name, "%s-rsd",thename);
     rightsd = inputspecdensity(name);
     if (!leftsd) {printf("probleme\n"); return 0;}
     if (!rightsd) {printf("probleme\n"); return 0;}
     */
    
#ifdef OUTPUTOCCUPATION
    if (_para_currentthread == 0) {
        calculateoccupation(occposition);
        sprintf(name, "%s-occupation",thename);
        p=openfile(name, "w");
        if (p && occupationfct) {
            n=0;
            while (n<rangedivisions) {
                fprintf(p, "%lf %lf\n",getrangespeed(n),occupationfct[n]);
                n++;
            }
        }
    }
#endif
#ifdef RMSSPECTRALCHANGE
    startingsample = 0;
    totalnumberofsamples = 1;
    outputtype = SPECTRAL;
    flagsampling = 0;
#endif
    if (headertrue) fprintf(header, "\n\n");

    // adjust sample numbers for each thread. Main thread adds to the data already accumulated, while subthreads do their own independent sampling that is later recombined.
    
    n = totalnumberofsamples - startingsample;
    if (_para_currentthread == 0) {
        m = n/numberofthreads + n%numberofthreads;
        totalnumberofsamples = startingsample + m;
    }
    else {
        m = n/numberofthreads;
        totalnumberofsamples = m;
        startingsample = 0;
    }

    // start main loop. This is the same for all threads, thename having been modified for subthreads.
    
    count = startingsample;
    srand ((unsigned int)time(NULL));
#ifdef LOOP
    while (count<totalnumberofsamples) {
#ifdef BRANCHING
        thebranch=1;
#endif
        totalnumber=sample();
        printf("Sample %d: %d rods, ", count, totalnumber);
        if (headertrue) fprintf(header,"Sample %d: %d rods, ", count, totalnumber);
        if (totalnumber<=1) return -1;
        printf("mean distance %lf\n",meandistance());
        if (headertrue) fprintf(header,"mean distance %lf\n",meandistance());
        
#ifdef EVOLVE
        
#ifdef BRANCHING
        while (thebranch <= numberofbranches) {
            printf("Branch %d\n ",thebranch);
            if (thebranch>1) retrieveroddistribution();
            branchingtime = totaltime + initialtime;
#endif
            printf("Initial evolution...\n");
            evolve(initialtime);
#ifdef BRANCHING
            saveroddistribution();
#endif
#ifdef SOLUTIONBYITERATION
            sol_calculateoccupation(initialtime);
#endif
            printf("Done.\n");
#ifdef VEFF
            if (count==0 && _para_currentthread==0) testparticleinit();
#endif
#ifdef DRAW
            erase(thename);
#endif
#endif
            printf("Time evolution...\n");
            n=0;
            while (n<numberofsteps+1) {
#ifdef GRAPH
#ifdef RMSSPECTRALCHANGE
                if (flagsampling == 0 && n%periodofsampling == 0 && numberofsteps-n>2*numberaveragingsteps) {
                    j=0;
                    flagsampling = 1;
                }
                if (flagsampling==2) {
                    sprintf(name, "%s-rms2",thename);
                    output(name,j,outputtype,n*timestep+initialtime,n);
                    j++;
                    if (j==numberaveragingsteps) {
                        flagsampling = 0;
                        sprintf(nameoth, "%s-rms1",thename);
                        printf("%d: %lf\n",n,calculatermsspectral(nameoth, name));
                    }
                }
                if (flagsampling==1) {
                    sprintf(name, "%s-rms1",thename);
                    output(name,j,outputtype,n*timestep+initialtime,n);
                    j++;
                    if (j==numberaveragingsteps) {
                        flagsampling = 2;
                        j=0;
                    }
                }
#else
#ifdef BRANCHING
                sprintf(name, "%s_%d-%d",thename,thebranch,n);
#else
                sprintf(name, "%s-%d",thename,n);
#endif
                output(name,count,outputtype,n*timestep+initialtime,n);
#endif
#endif
#ifdef DRAW
                drawconf(thename);
#endif
#ifdef EVOLVE
                printf("%d\n",n);
#ifdef DISPLAYMAXDISPLACEMENT
                printf("displacement: %lf\n",maxdisplacement);
#endif
                if (n<numberofsteps) evolve(timestep);
#endif
                n++;
#ifdef SOLUTIONBYITERATION
                if (n<numberofsteps+1) sol_calculateoccupation(timestep);
#endif
            }
            printf("Done.\n");
#ifdef VEFF
            if (count==0 && _para_currentthread==0) testparticleoutput(thename);
#endif
#ifdef BRANCHING
            thebranch++;
        }
#endif
        count++;
    }
#endif
    
    // recombine the threads

    if (_para_currentthread>0) para_sendmessageto(0, NULL);
    else {
        n=0;
        while (n<numberofthreads) {
            para_waitformessagefrom(n);
            n++;
        }
        recombinethreads(thename);
    }

    // do some calculations (in main thread)

    if (_para_currentthread==0) {
#ifdef EOS
        printf("Calculating Euler equation, full density / current...\n");
        checkeos(thename, eulerleft, eulerright, eulerrange, FULL);
        printf("Done.\n");
#endif
#ifdef EOSPARTIAL
        printf("Calculating Euler equation, partial integration of spectral density / current...\n");
        checkeos(thename, eulerleft, eulerright, eulerrange, PART);
        printf("Done.\n");
#endif
    }
    
    if (headertrue) {
        fprintf(header, "\n//======================\n");
        fclose(header);
    }
    
    // free memory (all threads)
    
    struct rod *r;
    while (rods) {
        r=rods->next;
#ifdef SOFTRODS
        free(rods->tableactiverods);
#endif
        free(rods);
        rods = r;
    }
#ifdef SOFTRODS
    struct activepair *a;
    while (activepairs) {
        a=activepairs->next;
        free(activepairs);
        activepairs = a;
    }
#endif
    if (collisiontree) freecollisiontree(collisiontree);
#ifdef OUTPUTOCCUPATION
    if (occupationfct) free(occupationfct);
#endif
#ifdef SOLUTIONBYITERATION
    if (exactoccupation) {
        n=0;
        while (n<rangedivisions) {
            if (exactoccupation[n]) free(exactoccupation[n]);
            n++;
        }
    }
#endif
#ifdef FROMINITIALGGE
    if (thetablesd) {
        n=0;
        while (n<rangedivisions) {
            if (thetablesd[n]) free(thetablesd[n]);
            n++;
        }
    }
#endif
    
#ifdef PERFORMANCE
    
#ifdef ACCELERATION
    free(accelerationtable);
#endif
#ifdef PERFORODTABLE
#ifndef PERFORODTABLEDIFF
    n=0;
    while (n<perfodivisions) {
        if (rodlengthtable[n]) free(rodlengthtable[n]);
        n++;
    }
    free(rodlengthtable);
#else
    free(rodlengthtablediff);
#endif
#endif
    
#endif
    // This is to read data for left and right spectral density; only needed for domain wall initial condition with out-sourced initial spectral density
    //    if (leftsd) free(leftsd);
    //    if (rightsd) free(rightsd);
    return 0;
}

