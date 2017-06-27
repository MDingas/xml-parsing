#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "interface.h"

float timedifference_msec(struct timeval t0, struct timeval t1)
{
    return (t1.tv_sec - t0.tv_sec) * 1000.0f + (t1.tv_usec - t0.tv_usec) / 1000.0f;
}

int main(){
        struct timeval start, end;
        float diff;

        char* names[] = {
                "snapshot_fev17",
                "snapshot_jan17",
                "snapshot_dec16"
        };

        printf("\n   Main start\n\n");
        gettimeofday(&start, 0);

        TAD_istruct list = init();

        list = load(list,3,names);
		if(list == NULL){
			printf("error loading\n");
			return 0;
		}

        printf("Total = %ld(59593)      ",all_articles(list));

        printf("Unique = %ld(19867)      ",unique_articles(list));

        printf("Revisions = %ld(40131)\n\n",all_revisions(list));


        printf("CONTRIBUTOR NAME\n");
        long number = 28903366;
        printf("%s (Bender the Bot)      ",contributor_name(number,list));
        number = 194203;
        printf("%s (Graham87)     ",contributor_name(number,list));
        number = 1000;
        printf("%s (null)\n\n",contributor_name(number,list));

        printf("ARTICLE TITLE\n");
        long id = 1111;
        printf("%s (Politics of American Samoa)        ",article_title(id,list));
        id = 25507;
        printf("%s (Roman Empire)\n",article_title(id,list));
        id = 15910;
        printf("%s (List of compositions by Johann Sebastian Bach)\n\n",article_title(id,list));

        printf("ARTICLE TIMESTAMP\n");
        long testid = 12;
        long testrevid = 763082287;
        printf("%s (2017-02-01T06:11:56Z)     ", article_timestamp(testid,testrevid,list));
        testid = 12;
        testrevid = 755779730;
        printf("%s (2016-12-20T04:02:33Z)     ", article_timestamp(testid,testrevid,list));
        testid = 12;
        testrevid = 4479730;
        printf("%s (null)\n\n", article_timestamp(testid,testrevid,list));


        long* top20 = top_20_largest_articles (list);
        printf("TOP 20 LARGEST ARTICLES\n");
        for (int i = 0; i < 20; i++)
            printf(" %ld",top20[i] );
        printf("\n 15910 23235 11812 28678 14604 23440 26847 25507 26909 18166 4402 14889 23805 25391 7023 13224 12108 13913 23041 18048\n");


        long* topN = top_N_articles_with_more_words (30, list);
        printf("\nTOP N ARTICLES WITH MORE WORDS\n");
        for (int i = 0; i < 30; i++)
            printf(" %ld",topN[i]);
        printf("\n 15910 25507 23235 11812 13224 26847 14889 7023 14604 13289 18166 4402 12157 13854 23805 25401 10186 23041 18048 16772 22936 28678 27069 9516 12108 13913 13890 21217 23440 25391\n");


        printf("\nTITLES WITH PREFIX\n");
        char** outputPrefix = titles_with_prefix("Anax",list);
        for(int i = 0; outputPrefix[i] != NULL; i++)
            printf("%s, ",outputPrefix[i]);
        printf("\nAnaxagoras, Anaxarchus, Anaximander, Anaximenes of Lampsacus, Anaximenes of Miletus,\n");


        printf("\nTOP 10\n");
        long* top10 = top_10_contributors(list);
        for(int i = 0; i < 10; i++)
            printf(" %ld",top10[i]);
        printf("\n 28903366 13286072 27823944 27015025 194203 212624 7852030 7328338 7611264 14508071\n");


        gettimeofday(&end, 0);
        diff = timedifference_msec(start, end);
        printf("\n Time ---> %f ms\n", diff);

		list = clean(list);


        return 0;
}

