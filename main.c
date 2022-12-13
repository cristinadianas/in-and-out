/*
Titlul proiectului: In&Out (aplicatie fitness)
Savin Cristina-Diana
Grupa 164
*/

#include <rlutil.h>
#include <stdio.h>
#include <stdlib.h>


#define PAHARE_DE_APA 8
#define NUMAR_MESE 5
#define CALORII 2000
#define GRASIMI 67
#define GLUCIDE 275
#define PROTEINE 50
#define SARE 6
#define NR_TOTAL_ZILE 100
// NR_INTERVALE trebuie sa divida NR_TOTAL_ZILE
// astfel, NR_TOTAL_ZILE sunt impartite in NR_INTERVALE intervale egale
#define NR_INTERVALE 10


enum actiuni
{
    iesi_din_program,
    // create
    create_aliment,
    create_workout,
    // read
    read_statistici,
    read_avertizari,
    read_alimente,
    read_workouturi,
    // update
    update_profil_al,
    update_profil_w,
    update_profil_apa,
    update_obiective,
    update_statistici,
    // delete
    delete_aliment,
    delete_workout,
};


typedef struct Aliment {
    char nume[100];
    int calorii;
    // masurata in kcal, per 100 grame produs
    float grasimi;
    float glucide;
    float proteine;
    float sare;
    // masurate in grame, per 100 grame produs
} Aliment;


typedef struct Workout {
    char nume[100];
    int calorii_arse;
} Workout;


typedef struct Profil {
    char nume[100];
    int *stare;

    int numar_mese_obiectiv : 4;
    int apa_obiectiv;
    int calorii_obiectiv;
    float grasimi_obiectiv;
    float glucide_obiectiv;
    float proteine_obiectiv;
    float sare_obiectiv;

    int numar_mese_ramase : 4;
    int apa_ramasa;
    int calorii_ramase;
    float grasimi_ramase;
    float glucide_ramase;
    float proteine_ramase;
    float sare_ramasa;
} Profil;


// reseteaza cantitatile ramase (ele ar trebui resetate zilnic la o ora fixa, dar momentan acest lucru se va face manual)
// nu afecteaza obiectivele
void resetareStatistici(Profil *p)
{
    p->numar_mese_ramase = p->numar_mese_obiectiv;
    p->apa_ramasa = p->apa_obiectiv;
    p->calorii_ramase = p->calorii_obiectiv;
    p->grasimi_ramase = p->grasimi_obiectiv;
    p->glucide_ramase = p->glucide_obiectiv;
    p->proteine_ramase = p->proteine_obiectiv;
    p->sare_ramasa = p->sare_obiectiv;
}


// returneaza nota referitoare la sanatatea mintala, in functie de inputul de la tastatura
int notaSanatateMintala(int *nr_zi)
{
    puts("Nu uita de sanatatea mintala!");
    printf("Introdu starea ta de spirit, sub forma unei note de la 1 la 10: ");
    int nota;
    scanf("%d", &nota);
    *nr_zi = *nr_zi + 1;
    return nota;
}


void afiseazaSanatateaMintala(Profil p)
{
    puts("\n-----------------------------------------------------------------------------\n");
    printf("!!! Felicitari !!!\n"
           "Ai completat provocarea noastra!\n\n"
           "Aceasta este reprezentarea grafica a starii tale de spirit "
           "din ultimele %d zile, impartita in %d intervale:\n",
           NR_TOTAL_ZILE, NR_INTERVALE);

    int i, j, nr_zile, media;
    for (i = 0; i < NR_INTERVALE; i++)
    {
        media = 0;
        nr_zile = NR_TOTAL_ZILE / NR_INTERVALE;
        for (j = i * nr_zile; j < (i + 1) * nr_zile; j++)
            media += p.stare[j];
        media /= nr_zile;

        // afisarea grafica a starii de spirit
        printf("Intervalul %d", i);
        if (i < 10)
            printf("    ");
        else if (i < 100)
            printf("   ");
        printf(": nota %d", media);
        if (media < 10)
            printf("    ");
        else if (media < 100)
            printf("   ");
        for (j = 0; j < media; j++)
            printf("____");
        printf("\n");
    }
    printf("\nSper ca am ajutat la imbunatatirea sanatatii tale mintale.\n");
}


// schimba obiectivele utilizatorului folosind obiectivele default
void setareObiectiveDefault(Profil *p)
{
    p->numar_mese_ramase += (NUMAR_MESE - p->numar_mese_obiectiv);
    p->numar_mese_obiectiv = NUMAR_MESE;

    p->apa_ramasa += (PAHARE_DE_APA - p->apa_obiectiv);
    p->apa_obiectiv = PAHARE_DE_APA;

    p->calorii_ramase += (CALORII - p->calorii_obiectiv);
    p->calorii_obiectiv = CALORII;

    p->grasimi_ramase += (GRASIMI - p->grasimi_obiectiv);
    p->grasimi_obiectiv = GRASIMI;

    p->glucide_ramase += (GLUCIDE - p->glucide_obiectiv);
    p->glucide_obiectiv = GLUCIDE;

    p->proteine_ramase += (PROTEINE - p->proteine_obiectiv);
    p->proteine_obiectiv = PROTEINE;

    p->sare_ramasa += (SARE - p->sare_obiectiv);
    p->sare_obiectiv = SARE;
}


// schimba obiectivele utilizatorului folosind input de la tastatura
void setareObiectiveTastatura(Profil *p)
{
    p->numar_mese_ramase += (NUMAR_MESE - p->numar_mese_obiectiv);
    p->numar_mese_obiectiv = NUMAR_MESE;

    int calorii_obiectiv_vechi = p->calorii_obiectiv;
    printf("Obiectiv caloric: ");
    scanf("%d", &p->calorii_obiectiv);
    p->calorii_ramase += (p->calorii_obiectiv - calorii_obiectiv_vechi);

    int grasimi_obiectiv_vechi = p->grasimi_obiectiv;
    printf("\nObiectiv grasimi (g): ");
    scanf("%f", &p->grasimi_obiectiv);
    p->grasimi_ramase += (p->grasimi_obiectiv - grasimi_obiectiv_vechi);

    int glucide_obiectiv_vechi = p->glucide_obiectiv;
    printf("\nObiectiv glucide (g): ");
    scanf("%f", &p->glucide_obiectiv);
    p->glucide_ramase += (p->glucide_obiectiv - glucide_obiectiv_vechi);

    int proteine_obiectiv_vechi = p->proteine_obiectiv;
    printf("\nObiectiv proteine (g): ");
    scanf("%f", &p->proteine_obiectiv);
    p->proteine_ramase += (p->proteine_obiectiv - proteine_obiectiv_vechi);

    int sare_obiectiv_vechi = p->sare_obiectiv;
    printf("\nObiectiv sare (g): ");
    scanf("%f", &p->sare_obiectiv);
    p->sare_ramasa += (p->sare_obiectiv - sare_obiectiv_vechi);

    int apa_obiectiv_vechi = p->apa_obiectiv;
    printf("\nObiectiv pahare de apa: ");
    scanf("%d", &p->apa_obiectiv);
    p->apa_ramasa += (p->apa_obiectiv - apa_obiectiv_vechi);

    puts("");
}


// returneaza un profil folosind informatiile dintr-un fisier
Profil fCreeazaProfil(FILE *fp)
{
    Profil p;

    fscanf(fp, "%100s", p.nume);

    p.stare = (int*) calloc(NR_TOTAL_ZILE, sizeof(int));
    if (p.stare == NULL)
    {
        printf("!!! Memoria nu a putut fi alocata.\n");
        exit(0);
    }

    int tip_obiective;
    fscanf(fp, "%d", &tip_obiective);

    // se pastreaza obiectivele default
    if (tip_obiective == 0)
        setareObiectiveDefault(&p);
    // se customizeaza obiectivele
    else if (tip_obiective == 1)
    {
        p.numar_mese_obiectiv = NUMAR_MESE;
        fscanf(fp, "%d", &p.calorii_obiectiv);
        fscanf(fp, "%f", &p.grasimi_obiectiv);
        fscanf(fp, "%f", &p.glucide_obiectiv);
        fscanf(fp, "%f", &p.proteine_obiectiv);
        fscanf(fp, "%f", &p.sare_obiectiv);
        fscanf(fp, "%d", &p.apa_obiectiv);
    }

    resetareStatistici(&p);

    return p;
}


// schimba obiectivele utilizatorului (fie folosind input de la tastatura, fie folosind obiectivele default)
void schimbaObiectivele(Profil *p)
{
    puts("0. Doresc sa folosesc obiectivele default.");
    puts("1. Doresc sa imi customizez obiectivele.");

    int decizie;
    scanf("%d", &decizie);
    if (decizie == 0)
    {
        setareObiectiveDefault(p);
        puts("Profilul a fost actualizat cu succes, folosind obiectivele default.");
    }
    else if (decizie == 1)
    {
        setareObiectiveTastatura(p);
        puts("Profilul a fost actualizat cu succes, folosind obiectivele customizate.");
    }
}


// returneaza un aliment folosind input dintr-un fisier
Aliment fCreeazaAliment(FILE *fp)
{
    Aliment a;
    fscanf(fp, "%100s", a.nume);
    fscanf(fp, "%d", &a.calorii);
    fscanf(fp, "%f", &a.grasimi);
    fscanf(fp, "%f", &a.glucide);
    fscanf(fp, "%f", &a.proteine);
    fscanf(fp, "%f", &a.sare);
    return a;
}


// returneaza un aliment folosind input de la tastatura
Aliment creeazaAliment()
{
    Aliment a;
    printf("Numele alimentului: ");
    scanf("%100s", a.nume);
    puts("");

    puts("Este timpul sa introduceti valorile nutritionale ale acestui aliment, raportate la 100 g de produs.");
    printf("- calorii: ");
    scanf("%d", &a.calorii);
    printf("- grasimi: ");
    scanf("%f", &a.grasimi);
    printf("- glucide: ");
    scanf("%f", &a.glucide);
    printf("- proteine: ");
    scanf("%f", &a.proteine);
    printf("- sare: ");
    scanf("%f", &a.sare);

    puts("\nAlimentul a fost adaugat cu succes.");

    return a;
}


// returneaza un workout folosind input drintr-un fisier
Workout fCreeazaWorkout(FILE *fp)
{
    Workout w;
    fscanf(fp, "%100s", w.nume);
    fscanf(fp, "%d", &w.calorii_arse);
    return w;
}


// returneaza un workout folosind input de la tastatura
Workout creeazaWorkout()
{
    Workout w;
    printf("Numele workoutului: ");
    scanf("%100s", w.nume);
    printf("Numarul de calorii arse cu ajutorul acestui workout: ");
    scanf("%d", &w.calorii_arse);
    puts("\nWorkoutul a fost adaugat cu succes.");
    return w;
}


// adauga un aliment in lista alimentelor
void adaugaAlimentInLista(Aliment *a, Aliment de_adaugat, int *no_alimente)
{
    *no_alimente = *no_alimente + 1;
    a = realloc(a, (*no_alimente) * sizeof(Aliment));
    if (a == NULL)
    {
        printf("!!! Memoria nu a putut fi alocata.\n");
        exit(0);
    }
    a[(*no_alimente) - 1] = de_adaugat;
}


// adauga un workout in lista workouturilor
void adaugaWorkoutInLista(Workout *w, Workout de_adaugat, int *no_workouts)
{
    *no_workouts  = *no_workouts + 1;
    w = realloc(w, (*no_workouts) * sizeof(Workout));
    if (w == NULL)
    {
        printf("!!! Memoria nu a putut fi alocata.\n");
        exit(0);
    }
    w[(*no_workouts) - 1] = de_adaugat;
}


// afiseaza cantitatile ramase de consumat pana la atingerea obiectivelor
// afiseaza 0 in cazul in care obiectivul a fost deja depasit
void afiseazaStatistici(Profil p)
{
    printf("In %d mese mai ai de consumat:\n", (p.numar_mese_ramase > 0)? p.numar_mese_ramase : 0);
    printf("- %d calorii;\n", (p.calorii_ramase > 0)? p.calorii_ramase : 0);
    printf("- %f g grasimi;\n", (p.grasimi_ramase > 0)? p.grasimi_ramase : 0);
    printf("- %f g glucide;\n", (p.glucide_ramase > 0)? p.glucide_ramase : 0);
    printf("- %f g proteine;\n", (p.proteine_ramase > 0)? p.proteine_ramase : 0);
    printf("- %f g sare;\n", (p.sare_ramasa > 0)? p.sare_ramasa : 0);
    printf("- %d pahare de apa.\n", (p.apa_ramasa > 0)? p.apa_ramasa : 0);
}


// afiseaza cantitatile pentru care obiectivul a fost depasit, mentionanad si cat de mult
void afiseazaAvertizari(Profil p)
{
    if (p.numar_mese_ramase > 0 && p.calorii_ramase > 0 && p.grasimi_ramase > 0 &&
        p.glucide_ramase > 0 && p.proteine_ramase > 0 && p.sare_ramasa > 0 && p.apa_ramasa > 0)
        puts("Nu exista avertizari.");
    else
    {
        if (p.numar_mese_ramase < 0)
            printf("Ati consumat %d mese in plus.\n", p.numar_mese_ramase * (-1));
        if (p.calorii_ramase < 0)
            printf("Ati consumat %d calorii in plus.\n", p.calorii_ramase * (-1));
        if (p.grasimi_ramase < 0)
            printf("Ati consumat %f g grasimi in plus.\n", p.grasimi_ramase * (-1));
        if (p.glucide_ramase < 0)
            printf("Ati consumat %f g glucide in plus.\n", p.glucide_ramase * (-1));
        if (p.proteine_ramase < 0)
            printf("Ati consumat %f g proteine in plus.\n", p.proteine_ramase * (-1));
        if (p.sare_ramasa < 0)
            printf("Ati consumat %f g sare in plus.\n", p.sare_ramasa * (-1));
        if (p.apa_ramasa < 0)
            printf("Ati consumat %d pahare de apa in plus.\n", p.apa_ramasa * (-1));
    }
}


//afiseaza lista alimentelor
void afiseazaListaAlimente(Aliment *a, int no_alimente)
{
    printf("Lista alimentelor disponibile:\n");
    int i;
    for (i = 0; i < no_alimente; i++)
        printf("%d. %s\n", i, a[i].nume);
}


//afiseaza lista workouturilor
void afiseazaListaWorkouturi(Workout *w, int no_workouts)
{
    printf("Lista workouturi disponibile:\n");
    int i;
    for (i = 0; i < no_workouts; i++)
        printf("%d. %s\n", i, w[i].nume);
}


// afiseaza informatiile unui aliment
void afiseazaAliment(Aliment a)
{
    printf("Numele alimentului: %s\n"
           "- calorii: %d kcal\n"
           "- grasimi: %f g\n"
           "- glucide: %f g\n"
           "- proteine: %f g\n"
           "- sare: %f g\n\n", a.nume, a.calorii, a.grasimi, a.glucide, a.proteine, a.sare);
}


// afiseaza informatiile unui workout
void afiseazaWorkout(Workout w)
{
    printf("Numele workoutului: %s\n"
           "- numarul de calorii arse cu ajutorul acestui workout: %d\n\n", w.nume, w.calorii_arse);
}


// afiseaza lista workouturilor
// in plus, in functie de inputul de la tastatura se pot afisa mai multe informatii despre un anumit workout
void afiseazaListaWorkouturiPlus(Workout *w, int no_workouts)
{
    afiseazaListaWorkouturi(w, no_workouts);
    int actiune;
    do {
    puts("\nDaca doresti sa afisezi informatiile unui workout, scrie indexul acestuia.\n"
         "Daca doresti sa te intorci la meniul principal, scrie -1.");
    printf("R: ");
    scanf("%d", &actiune);
    if (actiune != -1)
        afiseazaWorkout(w[actiune]);
    } while (actiune != -1);
}


// afiseaza lista alimentelor
// in plus, in functie de inputul de la tastatura se pot afisa mai multe informatii despre un anumit aliment
void afiseazaListaAlimentePlus(Aliment *a, int no_alimente)
{
    afiseazaListaAlimente(a, no_alimente);
    int actiune;
    do {
        puts("\nDaca doresti sa afisezi informatiile unui aliment, scrie indexul acestuia.\n"
             "Daca doresti sa te intorci la meniul principal, scrie -1.");
        printf("R: ");
        scanf("%d", &actiune);
        if (actiune != -1)
            afiseazaAliment(a[actiune]);
    } while (actiune != -1);

}


// returneaza indexul alimentului ales
int alegeAlimentul(Aliment *a, int no_alimente)
{
    puts("Te rog alege un aliment.");
    afiseazaListaAlimente(a, no_alimente);
    int alegere;
    printf("R: ");
    scanf("%d", &alegere);
    puts("");
    return alegere;
}


// returneaza indexul workoutului ales
int alegeWorkoutul(Workout *w, int no_workouts)
{
    puts("Te rog alege un workout:");
    afiseazaListaWorkouturi(w, no_workouts);
    int alegere;
    printf("R: ");
    scanf("%d", &alegere);
    puts("");
    return alegere;
}


// actualizeaza profilul utilizatorului in functie de alimentul consumat
void alimentConsumat(Profil *p, Aliment a)
{
    p->calorii_ramase -= a.calorii;
    p->grasimi_ramase -= a.grasimi;
    p->glucide_ramase -= a.glucide;
    p->proteine_ramase -= a.proteine;
    p->sare_ramasa -= a.sare;
    puts("Alimentul a fost consumat cu succes.");
}


// actualizeaza profilul utilizatorului in functie de workoutul efectuat
void workoutEfectuat(Profil *p, Workout w)
{
    p->calorii_ramase += w.calorii_arse;
    puts("Workoutul a fost efectuat cu succes.");
}


// in cadrul unei mese se consuma mai multe alimente si se actualizeaza profilul conform fiecaruia
void masaConsumata(Profil *p, Aliment *a, int no_alimente)
{
    p->numar_mese_ramase--;
    int actiune;
    do {
        alimentConsumat(p, a[alegeAlimentul(a, no_alimente)]);
        puts("0. Inapoi la meniul principal.");
        puts("1. Am consumat inca un aliment.");
        printf("R: ");
        scanf("%d", &actiune);
        puts("");
    } while (actiune);
    puts("Masa a fost consumata cu succes.");
}


// actualizeaza profilul utilizatorului prin consumarea unui pahar de apa
void beaApa(Profil *p)
{
    p->apa_ramasa--;
    puts("Ati consumat un pahar de apa cu succes.");
}


// sterge un aliment din lista alimentelor
void stergeAliment(Aliment *a, int index_sters, int *no_alimente)
{
    int i;
    for (i = index_sters; i < *no_alimente; i++)
        a[i] = a[i+1];
    *no_alimente = *no_alimente - 1;
    a = realloc(a, (*no_alimente) * sizeof(Aliment));
    if (a == NULL)
    {
        printf("!!! Memoria nu a putut fi alocata.\n");
        exit(0);
    }
    else printf("Alimentul a fost sters.\n");

}


//sterge un workout din lista workouturilor
void stergeWorkout(Workout *w, int index_sters, int *no_workouts)
{
    int i;
    for (i = index_sters; i < *no_workouts; i++)
        w[i] = w[i+1];
    *no_workouts = *no_workouts - 1;
    w = realloc(w, (*no_workouts) * sizeof(Workout));
    if (w == NULL)
    {
        printf("!!! Memoria nu a putut fi alocata.\n");
        exit(0);
    }
    else printf("Workoutul a fost sters.\n");

}


int main()
{
    FILE *fin;
    fin = fopen("tastatura.txt", "r");
    if (fin == NULL)
    {
        puts("!!! Eroare la deschiderea fisierului.");
        exit(1);
    }

    Profil user = fCreeazaProfil(fin);

    int nr_alimente, nr_workouturi, idx, nr_zi = 0;

    Aliment *alimente;
    fscanf(fin, "%d", &nr_alimente);
    alimente = (Aliment*) malloc (nr_alimente * sizeof(Aliment));
    if (alimente == NULL)
    {
        printf("!!! Memoria nu a putut fi alocata.\n");
        exit(0);
    }
    for (idx = 0; idx < nr_alimente; idx++)
        alimente[idx] = fCreeazaAliment(fin);

    Workout *workouturi;
    fscanf(fin, "%d", &nr_workouturi);
    workouturi = (Workout*) malloc (nr_workouturi * sizeof(Workout));
    if (workouturi == NULL)
    {
        printf("!!! Memoria nu a putut fi alocata.\n");
        exit(0);
    }
    for (idx = 0; idx < nr_workouturi; idx++)
        workouturi[idx] = fCreeazaWorkout(fin);


    printf("Buna, %s! Obiectivele tale au fost extrase din fisierul tastatura.txt. "
           "De asemenea, au fost extrase %d alimente si %d workouturi.\n\n"
           "Corp sanatos, minte sanatoasa. "
           "Completeaza provocarea de a trai sanatos %d de zile si observa "
           "cum ti se va imbunatati si sanatatea mintala. "
           "Pentru a-ti putea urmari progresul, te rog sa introduci zilnic o nota "
           "de la 1 la 10 care iti caracterizeaza starea de spirit in acea zi. "
           "Vei putea introduce aceasta nota atunci cand iti resetezi statisticile.\n"
           "\nStay fit, In&Out <3\n", user.nume, nr_alimente, nr_workouturi, NR_TOTAL_ZILE);
    int actiune;
    do {
        puts("\n-----------------------------------------------------------------------------\n");
        puts("MENIU\n");
        // create
        puts("1. Adauga un aliment in lista alimentelor disponibile.\n"
             "2. Adauga un workout in lista workouturilor disponibile.\n");
        // read
        puts("3. Afiseaza statisticile tale.\n"
             "4. Afiseaza avertizarile tale.\n"
             "5. Afiseaza lista alimentelor disponibile.\n"
             "6. Afiseaza lista workouturilor disponibile.\n");
        //update
        puts("7. Am consumat o masa!\n"
             "8. Am facut un workout!\n"
             "9. Am baut un pahar de apa!\n"
             "10. Schimba obiectivele.\n"
             "11. Reseteaza statisticile.\n");
        //delete
        puts("12. Sterge un aliment din lista alimentelor disponibile.\n"
             "13. Sterge un workout din lista workouturilor disponibile.\n");

        puts("0. Iesi din program :(\n");

        printf("R: ");
        scanf("%d", &actiune);
        puts("\n-----------------------------------------------------------------------------\n");

        switch (actiune)
        {
            case iesi_din_program:
                break;

            case create_aliment:
                adaugaAlimentInLista(alimente, creeazaAliment(), &nr_alimente);
                break;

            case create_workout:
                adaugaWorkoutInLista(workouturi, creeazaWorkout(), &nr_workouturi);
                break;

            case read_statistici:
                afiseazaStatistici(user);
                break;

            case read_avertizari:
                afiseazaAvertizari(user);
                break;

            case read_alimente:
                afiseazaListaAlimentePlus(alimente, nr_alimente);
                break;

            case read_workouturi:
                afiseazaListaWorkouturiPlus(workouturi, nr_workouturi);
                break;

            case update_profil_al:
                if (!nr_alimente) puts("!!! Momentan nu ai adaugat niciun aliment.");
                else masaConsumata(&user, alimente, nr_alimente);
                break;

            case update_profil_w:
                if (!nr_workouturi) puts("!!! Momentan nu ai adaugat niciun workout.");
                else workoutEfectuat(&user, workouturi[alegeWorkoutul(workouturi, nr_workouturi)]);
                break;

            case update_profil_apa:
                beaApa(&user);
                break;

            case update_obiective:
                schimbaObiectivele(&user);
                break;

            case update_statistici:
                resetareStatistici(&user);
                puts("Statisticile au fost resetate cu succes.\n");
                user.stare[nr_zi] = notaSanatateMintala(&nr_zi);
                if (nr_zi == NR_TOTAL_ZILE)
                {
                    afiseazaSanatateaMintala(user);
                    actiune = 0;
                }
                break;

            case delete_aliment:
                stergeAliment(alimente, alegeAlimentul(alimente, nr_alimente), &nr_alimente);
                break;

            case delete_workout:
                stergeWorkout(workouturi, alegeWorkoutul(workouturi, nr_workouturi), &nr_workouturi);
                break;

            default:
                puts("!!! Optiunea aleasa nu este valida.");
        }
    } while (actiune);

    puts("See you soon!\nStay fit, In&Out. <3");
    free(alimente);
    free(workouturi);
    fclose(fin);

    return 0;
}
