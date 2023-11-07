// ref
// https://stackoverflow.com/questions/14277477/c-boolean-expression-evaluator

#include "api.h"
#include <stdio.h>
#include <stdlib.h>

#define MODULE 107374585931
// setting for similarity problem
// By this module and the string to int hashing, no collision will happen
// The total number of unique hashing of string is 191674
// With this setting, the avg probing number in hashtable is 2
#define CONTENTHASHMAX 709 //200003

// setting for group analysis
#define NAMEHASHMAX 2437

// The testdata only contains the first 100 mails (mail1 ~ mail100)
// and 2000 queries for you to debug.
typedef struct contenthashnode {
	long long int key;
	struct contenthashnode *next;
} ContentHashnode_t;

typedef struct contenthasharray {
	ContentHashnode_t *head;   
	ContentHashnode_t *tail;   
} ContentHasharray_t;

typedef struct hashtable {
	ContentHasharray_t *hasharray;
	int size;
} ContentHashtable_t;

ContentHashtable_t *initContentHashTable(void) {
	ContentHashtable_t *hashtable = (ContentHashtable_t*)malloc(sizeof(ContentHashtable_t));
	hashtable->hasharray = (ContentHasharray_t*)malloc(CONTENTHASHMAX * sizeof(ContentHasharray_t));
	for (int i = 0; i < CONTENTHASHMAX; i++){
		hashtable->hasharray[i].head = NULL;
		hashtable->hasharray[i].tail = NULL;
	}
	hashtable->size = 0;
	return hashtable;
}

void resetContentHashTable(ContentHashtable_t *hashtable){
	for (int i = 0; i < CONTENTHASHMAX; i++){
		hashtable->hasharray[i].head = NULL;
		hashtable->hasharray[i].tail = NULL;
	}
	hashtable->size = 0;
}

int contenthashcode(long long int key){
	return (key % CONTENTHASHMAX);
}

int findContentHashTable(ContentHashnode_t *list, long long int key){
	int retval = 0;
	ContentHashnode_t *temp = list;
	while (temp != NULL) {
		if (temp->key == key){
			return retval;
		}
  		temp = temp->next;
		retval++;
	}
	return -1;
}

int directFindContentHashTable(ContentHashtable_t *hashtable, long long int key){
	int index = contenthashcode(key);  
	ContentHashnode_t* list = hashtable->hasharray[index].head;
	ContentHashnode_t *temp = list;
	int retval = 0;
	while (temp != NULL) {
		if (temp->key == key){
			return retval;
		}
  		temp = temp->next;
		retval++;
	}
	return -1;
}

void insertContentHashTable(ContentHashtable_t *hashtable, long long int key){
	int index = contenthashcode(key);  
 
	ContentHashnode_t* list = hashtable->hasharray[index].head;
	ContentHashnode_t *item = (ContentHashnode_t*) malloc(sizeof(ContentHashnode_t));
	item->key = key;
	item->next = NULL;
 
	if (list == NULL){
		//printf("Inserting %lld(key) \n", key);
		hashtable->hasharray[index].head = item;
		hashtable->hasharray[index].tail = item;
		hashtable->size++;
	} else {
		int find_index = findContentHashTable(list, key); 
		if (find_index == -1) {
			//printf("Inserting %lld(key) at tail\n", key);
			hashtable->hasharray[index].tail->next = item;
			hashtable->hasharray[index].tail = item;
			hashtable->size++;
		} else {
			free(item);
			//printf("Already exist %lld(key) at %d\n", key, find_index);
		}
	}
}

int char2int(char c){
	if ('0' <= c && c <= '9') return c - '0' + 1;
    else if ('a' <= c && c <= 'z') return c - 'a' + 11;
    else if ('A' <= c && c <= 'Z') return c - 'A' + 11;
    return -1;
}

long long int string2int(char *string, int len){
	long long int hashing = 0;
	int temp_chr_int;
	for (int i = 0; i < len; i++){
		temp_chr_int = char2int(string[i]);
		hashing = (hashing * 37 + temp_chr_int) % MODULE;
	}
	return hashing;
}

int isAlphaNumeric(char c){
    if ('0' <= c && c <= '9') return 1;
    else if ('a' <= c && c <= 'z') return 1;
    else if ('A' <= c && c <= 'Z') return 1;
    return 0;
}

void tokenizer(char *string, int *start, int *length){
    int loc = -1;
    int len = 0;
    int now = 0;
    while (string[now] != '\0') {
        if (isAlphaNumeric(string[now])){
            loc = now;
            ++now;
            ++len;
            while (isAlphaNumeric(string[now])){
                ++now;
                ++len;
            }
            *start = loc;
            *length = len;
            return;
        }
        ++now;
    }
    *start = loc;
    *length = len;
    return;
}

void printstring(char *string, int len){
    for (int i = 0; i < len; i++){
        printf("%c", string[i]);
    }
    printf(" ");
}

ContentHashtable_t *mail_to_hash(mail *single_mail){
	ContentHashtable_t *hashtable = initContentHashTable();
	long long int hashing_value;

	int start, len;
    int loc = 0;
    while (1){
        tokenizer(&(single_mail->content[loc]), &start, &len); // the vaild string will be from loc+start with length of len
        if (start == -1) break;
		//printstring(&(single_mail->content[loc+start]), len);
        hashing_value = string2int(&(single_mail->content[loc + start]), len);
		insertContentHashTable(hashtable, hashing_value);
        loc += (start+len+1);
    }

	loc = 0;
    while (1){
        tokenizer(&(single_mail->subject[loc]), &start, &len); // the vaild string will be from loc+start with length of len
        if (start == -1) break;
		//printstring(&(single_mail->subject[loc+start]), len);
        hashing_value = string2int(&(single_mail->subject[loc + start]), len);
		insertContentHashTable(hashtable, hashing_value);
        loc += (start+len+1);
    }
	return hashtable;
}

double calc_mail_similarity(ContentHashtable_t *hashtable_query, ContentHashtable_t *hashtable_target){
	ContentHashnode_t *list, *temp;
	long long int key;
	int find;
	double intersect = 0.0;
	double similarity;
	for (int index = 0; index < CONTENTHASHMAX; index++){
		list = hashtable_query->hasharray[index].head;
		temp = list;
		if (temp == NULL) continue;

		while (temp != NULL) {
			key = temp->key;
			find = findContentHashTable(hashtable_target->hasharray[index].head, key);
			if (find != -1) {
				intersect += 1;
			}
			temp = temp->next;
		}
	}
	similarity = intersect / (hashtable_query->size + hashtable_target->size - intersect);
	return similarity;
}

void FIND_SIMILAR_QUERY(int mid, double threshold, int n_mails, mail *mails, ContentHashtable_t *MailContentHashTable[], int *answer_array, int *answer_num){
	double similarity;
	ContentHashtable_t *hashtable_query;
	hashtable_query = MailContentHashTable[mid];

	for (int i = 0; i < n_mails; i++){
		if (i != mid){
			similarity = calc_mail_similarity(hashtable_query, MailContentHashTable[i]);

			if (similarity > threshold) {
				answer_array[*answer_num] = i;
				*answer_num += 1;
			}
			//printf("%d %lf\n", i, similarity);
		}
	}
}

typedef struct namehashnode {
	long long int key;
	int value;
	struct namehashnode *next;
} NameHashnode_t;

typedef struct namehasharray {
	NameHashnode_t *head;   
	NameHashnode_t *tail;   
} NameHasharray_t;

typedef struct namehashtable {
	NameHasharray_t *hasharray;
	int size;
} NameHashtable_t;

NameHashtable_t *initNameHashTable(void) {
	NameHashtable_t *hashtable = (NameHashtable_t*)malloc(sizeof(NameHashtable_t));
	hashtable->hasharray = (NameHasharray_t*)malloc(NAMEHASHMAX * sizeof(NameHasharray_t));
	for (int i = 0; i < NAMEHASHMAX; i++){
		hashtable->hasharray[i].head = NULL;
		hashtable->hasharray[i].tail = NULL;
	}
	hashtable->size = 0;
	return hashtable;
}

int findNameHashTable(NameHashnode_t *list, long long int key){
	int retval = 0;
	NameHashnode_t *temp = list;
	while (temp != NULL) {
		if (temp->key == key){
			return retval;
		}
  		temp = temp->next;
		retval++;
	}
	return -1;
}

int findValueNameHashTable(NameHashtable_t *hashtable, long long int key){
	int index = namehashcode(key);
	NameHashnode_t *list = hashtable->hasharray[index].head;
	int value = 0;
	NameHashnode_t *temp = list;
	while (temp != NULL) {
		if (temp->key == key){
			value = temp->value;
			return value;
		}
  		temp = temp->next;
	}
	return -1;
}

int namehashcode(long long int key){
	return (key % NAMEHASHMAX);
}

void insertNameHashTable(NameHashtable_t *hashtable, long long int key, int value){
	int index = namehashcode(key);
	NameHashnode_t *list = hashtable->hasharray[index].head;
	NameHashnode_t *item = (NameHashnode_t*) malloc(sizeof(NameHashnode_t));
	item->key = key;
	item->value = value;
	item->next = NULL;
 
	if (list == NULL){
		hashtable->hasharray[index].head = item;
		hashtable->hasharray[index].tail = item;
		hashtable->size++;
	} else {
		int find_index = findNameHashTable(list, key); 
		if (find_index == -1) {
			hashtable->hasharray[index].tail->next = item;
			hashtable->hasharray[index].tail = item;
			hashtable->size++;
		} else {
			free(item);
		}
	}
}

NameHashtable_t *precomputeNameHashTable(void){
	int uniqueName = 560;
	long long int uniqueNameHashing[560] = {38283804, 1149646, 38478000, 25314911, 92028065600, 65920710424, 849349, 1221384182, 804864418, 1141226, 29223059592, 54940225, 57534226159, 102830627975, 43711667, 85626254291, 2040526886, 1541227, 52410533644, 39084299, 46772599, 27031828, 932748, 55891993, 33290323929, 1220656484, 1039907, 1648028, 21865017, 725400, 808511431, 21855892, 55885834, 1372469471, 28841441, 1048373, 1434678698, 2488311731, 2115351856, 1034026, 24963047, 1775837265, 21730, 22060643, 1363415197, 923544546, 1192336, 83206574088, 590592, 26834285, 43707426, 1643207024, 1149638, 1277448704, 40266481472, 41822077, 7147429932, 92028065132, 821630582, 1445416, 29919896831, 68023818711, 40362003, 56822391, 34528448, 53469623, 1434874127, 888025, 78333741252, 936005537, 1839069, 29257478, 60766979, 1047975, 34161325901, 1489903, 99123650429, 56816175, 1056052, 21855293, 793368, 67529339788, 82911809059, 880230755, 23951151, 37048781, 38921, 1989114258, 954847, 1099140885, 34631901591, 48341194, 808511453, 1067401763, 59962359646, 790738, 2040228444, 23268336, 72903766823, 1047716, 2047742700, 587890, 23280670, 29304294, 1616986813, 91575845350, 65980628632, 1081974994, 1478283692, 1067605969, 1617275898, 59832260584, 73898680672, 6091111763, 792144, 53529834758, 798843, 88315028495, 34902216265, 49327584, 1554347, 806479798, 40863, 1693522399, 20998514358, 38282418, 57507991545, 28867461, 52132854309, 40053692503, 75192784514, 1700954872, 32336133883, 59834029323, 1484866, 1712559195, 66138533571, 923592178, 49511918, 55141444, 854178685, 41832211, 34624699300, 38072474, 816510760, 2487813637, 44099077, 28106, 42541038, 955102, 42536778, 1270055950, 25359909, 57534346409, 1100923739, 1028652, 790997, 31604600885, 53067100, 923536568, 25505078, 24951961, 105624890006, 21212235, 17365, 25661263, 76182195089, 55954691, 78065221280, 822393252, 1157650, 629473, 634683, 71672664073, 1028621, 21757047, 876639, 21797801, 2256263021, 38073844, 23265907, 38789964, 43015103312, 54938819, 1970631222, 29657078, 39953475, 45172264209, 936005783, 43696271, 2059171112, 1250944, 34322789, 1485490169, 1372813712, 1163193, 17690228756, 43024277, 1208264985, 68260238, 76508246722, 576779, 30688, 1131368329, 808645852, 73638250793, 62397039734, 39963304, 21609380, 23291922, 30698, 804034, 53073282430, 62437881, 735655, 52385506165, 40110353732, 1162963291, 62640517, 28848696, 97170583397, 31929, 23620149, 1547791979, 39956352, 40040166596, 31420828, 923537562, 52102780163, 674619, 27216619, 1461851, 587732, 35363180184, 60954168, 1700557570, 574188, 1555179242, 99993428689, 65101458201, 44977256486, 23943686, 82054951469, 43702024, 54946609, 43696295, 76238355210, 21858575, 27015524, 992773692, 693761, 28340, 1333206, 1446488865, 923532406, 1439388, 57262027738, 1643007211, 43701734, 50025478, 34162942357, 861496221, 15971, 923596568, 32767509799, 44482328006, 25142410, 2067990321, 49504098, 55977001007, 22346, 38276794, 41796, 927722, 1006646, 1001471, 33294576, 1068085267, 29108176746, 27538334, 53766689, 999552795, 1112457, 23929669, 2032798559, 891798, 57805888531, 812808995, 2045306115, 43889202, 55650564, 31136214, 1584351448, 36984124095, 1250322, 29191, 752373, 42018, 2032826409, 1631964795, 24960679, 37712759297, 31277715392, 2109871028, 943287228, 66794824645, 32766547701, 27036957, 935999580, 76397831066, 1157321, 1858780, 76788210560, 1067400666, 1079897, 21597775, 37696305434, 43015103164, 29271321, 1191822, 1135800, 818076658, 1096054839, 29557202, 992895357, 1858323, 76516184660, 730547, 60094094096, 34330966, 1242582, 1082381109, 38059137, 48209760, 104743015569, 47542972583, 31928, 57809245330, 29257617, 21971806, 39084295, 2045496282, 693543, 38797956, 60094101496, 1270599597, 1056402, 629437, 28845265, 744414, 693267, 40144315, 15588, 29944417199, 28041658, 24960555, 36043865775, 1546500, 1018900982, 29091603927, 40553886460, 55278727, 26442, 42820296, 23291942, 28970095680, 46285383, 1416500911, 1284956670, 76394126008, 34528620, 34656114304, 590942, 72648827880, 73183166591, 21610235, 27822, 888011, 78948774972, 1631207898, 98499563282, 1616986863, 1837965099, 1824745032, 1157597, 15945, 1435227902, 1616964039, 26839962, 1296732909, 53759988, 53506400754, 1466630, 804995987, 7506933503, 1574018417, 23483306, 30857, 45570419, 34331484, 40150955, 1554738, 841687, 72669918760, 75683514576, 3819288794, 17007, 1434324103, 38740064539, 44402995, 1416444930, 877231, 63653927, 73184051630, 2068785951, 2047326400, 29328, 1435171046, 23924193, 1624172570, 49529634, 38774, 26822556, 55961044, 1029022, 59831708818, 5787223528, 1616971502, 55683763, 1445714412, 57268303253, 49328633, 71453645594, 30984107, 1547692181, 33300677, 1099002760, 1000177073, 27230, 43679891, 65164515031, 790466507, 74977561721, 804644506, 53788441, 38276770, 2102115606, 71672664537, 40148143, 1181135, 42227334, 1277439441, 93154226228, 34909309258, 54700708140, 40161711, 2255912711, 24954685, 1141461, 1574018421, 1616411874, 78320140459, 41828128, 32462455, 70947579530, 37093475, 85954324818, 2068003765, 1837671034, 39292, 1242185, 56814386, 47995301, 32752199262, 1775837396, 21756644, 54941261, 42536926, 39956335, 59828461450, 67530910432, 1686415402, 60354534073, 646589, 2203651376, 3642692593, 80580806012, 1181143, 62397030192, 92028065604, 574090, 590938, 81039465601, 55345512, 54966631143, 1453597, 25306905, 23641471206, 34161131623, 34521241, 59203568949, 821787161, 1642478, 57523528, 1839614, 944008372, 49021455153, 60382309666, 1236932, 75691315138, 674606, 805828709, 22263873, 25494575, 59827961286, 57658689, 43473453302, 38072608, 33418713937, 38081053, 54966630568, 2047382986, 1616989601, 845723, 29790792152, 1181149, 57024028, 21734, 25670995, 31724571529, 23773164, 2325208886, 2045508849, 949574712, 588496, 36212592292, 102735142075, 804822142, 23934658, 1962930275, 1312081, 57525317, 35141963948, 1453011, 1989716380, 28320, 32201, 44404067284, 816805442, 55044523, 43703536, 41639, 40131, 1977923704, 885397172, 27543643, 58028386, 55694450, 992673429, 43695091};
	NameHashtable_t *hashtable = initNameHashTable();
	for (int i = 0; i < uniqueName; i ++){
		insertNameHashTable(hashtable, uniqueNameHashing[i], i);
	}
	return hashtable;
}

long long int string2intwithoutlen(char *string){
	long long int hashing = 0;
	int temp_chr_int;
	int loc = 0;
	while (string[loc] != '\0'){
		temp_chr_int = char2int(string[loc]);
		hashing = (hashing * 37 + temp_chr_int) % MODULE;
		loc += 1;
	}
	return hashing;
}

void nodeDFS(int adjacency_list[560][560], int *adjacency_list_num, int *reached, int v, int *current_node_num) {
	reached[v] = 1;
	*current_node_num += 1;
	for (int i = 0; i < adjacency_list_num[v]; i++){
		if (!reached[adjacency_list[v][i]]){
			nodeDFS(adjacency_list, adjacency_list_num, reached, adjacency_list[v][i], current_node_num);
		}
	}
}

void count_in_graph(int adjacency_list[560][560], int *adjacency_list_num, int node_num, int *reached, int *answer_array){
	for (int i = 0; i < node_num; i++){
		reached[i] = 0;
	}
	int counts = 0;
	int current_node_num = 0;
	int max_current_node_num = 0;
	for (int i = 0; i < node_num; i++){
		if (reached[i] == 0){
			current_node_num = 0;
			nodeDFS(adjacency_list, adjacency_list_num, reached, i, &current_node_num);
			if (current_node_num > max_current_node_num){
				max_current_node_num = current_node_num;
			}
			if (current_node_num > 1){
				counts += 1;
			}
		}
	}
	answer_array[0] = counts;
	answer_array[1] = max_current_node_num;
}

void GROUP_ANALYZE_QUERY(int *mids, int mid_len, int n_mails, mail *mails, NameHashtable_t *name_hashtable, int *remapping, int adjacency_list[560][560], int *adjacency_list_num, int *reached, int *answer_array){
	int from, to, mapped_from, mapped_to;
	for (int i = 0; i < 560; i ++){
		remapping[i] = -1;
		adjacency_list_num[i] = 0;
		reached[i] = 0;
	}

	int remapping_value = 0; // will be the number of node involved
	for (int i = 0; i < mid_len; i++){
		//printf("%s ", mails[mids[i]].from);
		from = findValueNameHashTable(name_hashtable, string2intwithoutlen(mails[mids[i]].from));
		if (remapping[from] == -1){
			remapping[from] = remapping_value;
			remapping_value += 1;
		}
		//printf("%s", mails[mids[i]].to);
		//printf("\n");
		to = findValueNameHashTable(name_hashtable, string2intwithoutlen(mails[mids[i]].to));
		if (remapping[to] == -1){
			remapping[to] = remapping_value;
			remapping_value += 1;
		}
		mapped_from = remapping[from];
		mapped_to = remapping[to];
		if (mapped_to == mapped_from){
			continue;
		}

		// add to adjacency list
		adjacency_list[mapped_from][adjacency_list_num[mapped_from]] = mapped_to;
		adjacency_list[mapped_to][adjacency_list_num[mapped_to]] = mapped_from;
		adjacency_list_num[mapped_from] += 1;
		adjacency_list_num[mapped_to] += 1;
	}
	/*
	for (int i = 0; i < remapping_value; i++){
		printf("%d || ", i);
		for (int j = 0; j < adjacency_list_num[i]; j++){
			printf("%d ", adjacency_list[i][j]);
		}
		printf("\n");
	}
	*/
	
	count_in_graph(adjacency_list, adjacency_list_num, remapping_value, reached, answer_array);
}

/////////////////////////
int precedence(char c){
    if (c == '|') return 3;
    else if (c == '&') return 2;
    else if (c == '!') return 1;
    else return 0;
}

int left_association(char c){
    if ((c == '|') || (c == '&')) return 1;
    else if (c == '!') return 0;
    else return 0;
}

int isnumoralpha(char c){
	if ('0' <= c && c <= '9') return 1;
    else if ('a' <= c && c <= 'z') return 1;
    else if ('A' <= c && c <= 'Z') return 1;
    return 0;
}

int isoperator(char c){
    if (c == '|' || c == '&' || c == '!') return 1;
    else return 0;
}

int toPostFix(char *input, char *output, ContentHashtable_t *hashtable){
    char *strpos = input;
    char *strend = input + strlen(input);
    char c; 
    char *outpos = output;
	char *token_start;

    char stack[32];
    int sl = 0;
    char sc;
	int token_len, find, bit;
	long long int key;


    while(strpos < strend)   {
        c = *strpos;
		token_start = strpos;
		token_len = 1;
		++strpos;
        if(isnumoralpha(c))  {
			c = *strpos;
			while(isnumoralpha(c)){
				++strpos;
				token_len += 1;
				c = *strpos;
			}

			//printstring(token_start, token_len);
			key = string2int(token_start, token_len);
			find = directFindContentHashTable(hashtable, key);
			if (find == -1) bit = 0;
			else bit = 1;
			//printf("find = %d\n", find);

            *outpos = bit + '0'; 
            ++outpos;
        }
        else if(isoperator(c))  {
            while(sl > 0)    {
                sc = stack[sl - 1];
                if (isoperator(sc) && ((left_association(c) && (precedence(c) >= precedence(sc))) || (precedence(c) > precedence(sc)))){
                    *outpos = sc;
                    ++outpos;
                    sl--;
                }
                else{
                    break;
                }
            }
            stack[sl] = c;
            ++sl;
        }
        else if(c == '(')   {
            stack[sl] = c;
            ++sl;
        }
        else if(c == ')')    {
            int pe = 0;
            while(sl > 0)     {
                sc = stack[sl - 1];
                if(sc == '(')    {
                    pe = 1;
                    break;
                }
                else  {
                    *outpos = sc;
                    ++outpos;
                    sl--;
                }
            }
            sl--;
            if(sl > 0)   {
                sc = stack[sl - 1];
            }
        }
    }
    while(sl > 0)  {
        sc = stack[sl - 1];
        *outpos = sc;
        ++outpos;
        --sl;
    }
    *outpos = 0;
    return 1;
}

int expression_parser(char * expr, ContentHashtable_t *hashtable){
    char output[50] = {0};
    char * op;
    int temp;
    char part1[50], part2[50];

    if(!toPostFix(expr, output, hashtable)) return 0;
    while (strlen(output) > 1) {
        op = &output[0];
        while (!isoperator(*op) && *op != '\0')
          op++;
        if (*op == '\0') {
          return 0;
        }
        else if (*op == '!') {
            temp = !(*(op-1) - 48);
            *(op-1) = '\0';
        }
        else if(*op == '&') {
            temp = (*(op-1) - 48) && (*(op-2) - 48);
            *(op-2) = '\0';
        }
        else if (*op == '|') {
            temp = (*(op-1) - 48) || (*(op-2) - 48);
            *(op-2) = '\0';
        }
        memset(part1, 0, sizeof(part1));
        memset(part2, 0, sizeof(part2));
        strcpy(part1, output);
        strcpy(part2, op+1);
        memset(output, 0, sizeof(output));
        strcat(output, part1);
        strcat(output, ((temp==0) ? "0" : "1"));
        strcat(output, part2);
    }
    return *output - '0';
}

/////////////////////////

void EXPRESSION_MATCH_QUERY(char *expression, int n_mails, ContentHashtable_t *MailContentHashTable[], int *answer_array, int *answer_len){
	//printf("%s\n", expression);
	int res;
	for (int i = 0; i < n_mails; i++){
		res = expression_parser(expression, MailContentHashTable[i]);
		if (res == 1){
			//printf("%d ", i);
			answer_array[*answer_len] = i;
			*answer_len += 1;
		}

	}
	//printf("\n");
}

void generate_output(int idx, int *answer_array, int answer_len){
	printf("%d: ", idx);
	for (int j=0;j<answer_len;j++){
		printf("%d ",answer_array[j]);
	}
	printf("\n");
}

int n_mails, n_queries;
mail *mails;
query *queries;

int main(void) {
	api.init(&n_mails, &n_queries, &mails, &queries);

	// make hashtable for every mail
	/*
	ContentHashtable_t *MailContentHashTable[10000];
	for (int i = 0; i < n_mails; i++){
		MailContentHashTable[i] = mail_to_hash(&(mails[i]));
	}
	*/

	// 3333
	NameHashtable_t *name_hashtable = precomputeNameHashTable();
	int adjacency_list[560][560];
	int adjacency_list_num[560];
	int remapping[560];
	int reached[560];
	// 333

	int answer_array[n_mails];
	int answer_len = 0;
	int expression_counts = 0;
	int similarity_counts = 0;

	for(int i = 0; i < n_queries; i++)
		if(queries[i].type == find_similar){
			continue;
			if (similarity_counts <= 10000){
				answer_len = 0;
				//FIND_SIMILAR_QUERY(queries[i].data.find_similar_data.mid, queries[i].data.find_similar_data.threshold, n_mails, mails, MailContentHashTable, answer_array, &answer_len);
				api.answer(queries[i].id, answer_array, answer_len);
				similarity_counts += 1;
			}
		}
		else if(queries[i].type == group_analyse){
			if (queries[i].reward > 0.200){
				GROUP_ANALYZE_QUERY(queries[i].data.group_analyse_data.mids, queries[i].data.group_analyse_data.len, n_mails, mails, name_hashtable, remapping, adjacency_list, adjacency_list_num, reached, answer_array);
				api.answer(queries[i].id, answer_array, 2);
			}
		}
		else if (queries[i].type == expression_match){
			continue;
			if (expression_counts <= 10000){
				answer_len = 0;
				//EXPRESSION_MATCH_QUERY(queries[i].data.expression_match_data.expression, n_mails, MailContentHashTable, answer_array, &answer_len);
				api.answer(queries[i].id, answer_array, answer_len);
				expression_counts += 1;
			}
		}
  return 0;
}
