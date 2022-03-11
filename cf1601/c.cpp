#include <bits/stdc++.h>
 
using namespace std; 

namespace IO {
    const int BUFFER_SIZE = 1 << 15;
 
    char input_buffer[BUFFER_SIZE];
    int input_pos = 0, input_len = 0;
 
    char output_buffer[BUFFER_SIZE];
    int output_pos = 0;
 
    char number_buffer[100];
    uint8_t lookup[100];
 
    void _update_input_buffer() {
        input_len = fread(input_buffer, sizeof(char), BUFFER_SIZE, stdin);
        input_pos = 0;
 
        if (input_len == 0)
            input_buffer[0] = EOF;
    }
 
    inline char next_char(bool advance = true) {
        if (input_pos >= input_len)
            _update_input_buffer();
 
        return input_buffer[advance ? input_pos++ : input_pos];
    }
 
    template<typename T>
    inline void read_int(T &number) {
        bool negative = false;
        number = 0;
 
        while (!isdigit(next_char(false)))
            if (next_char() == '-')
                negative = true;
 
        do {
            number = 10 * number + (next_char() - '0');
        } while (isdigit(next_char(false)));
 
        if (negative)
            number = -number;
    }
 
    template<typename T, typename... Args>
    inline void read_int(T &number, Args &... args) {
        read_int(number);
        read_int(args...);
    }
 
    inline void read_double(double &number) {
        bool negative = false;
        number = 0;
 
        while (!isdigit(next_char(false)))
            if (next_char() == '-')
                negative = true;
 
        do {
            number = 10 * number + (next_char() - '0');
        } while (isdigit(next_char(false)));
 
        if (next_char(false) == '.') {
            next_char();
 
            for (double multiplier = 0.1; isdigit(next_char(false)); multiplier *= 0.1)
                number += multiplier * (next_char() - '0');
        }
 
        if (negative)
            number = -number;
    }
 
    void _flush_output() {
        fwrite(output_buffer, sizeof(char), output_pos, stdout);
        output_pos = 0;
    }
 
    inline void write_char(char c) {
        if (output_pos == BUFFER_SIZE)
            _flush_output();
 
        output_buffer[output_pos++] = c;
    }
 
    template<typename T>
    inline void write_int(T number, char after = '\0') {
        if (number < 0) {
            write_char('-');
            number = -number;
        }
 
        int length = 0;
 
        while (number >= 10) {
            uint8_t lookup_value = lookup[number % 100];
            number /= 100;
            number_buffer[length++] = (lookup_value & 15) + '0';
            number_buffer[length++] = (lookup_value >> 4) + '0';
        }
 
        if (number != 0 || length == 0)
            write_char(number + '0');
 
        for (int i = length - 1; i >= 0; i--)
            write_char(number_buffer[i]);
 
        if (after)
            write_char(after);
    }
 
    inline void write_double(double number, char after = '\0', int places = 6) {
        if (number < 0) {
            write_char('-');
            number = -number;
        }
 
        assert(number <= 9e18);
 
        // Round up the number according to places.
        number += 0.5 * pow(0.1, places);
        long long floored = (long long) number;
 
        if (floored <= INT_MAX)
            write_int((int) floored);
        else
            write_int(floored);
 
        number -= floored;
 
        if (places > 0) {
            write_char('.');
 
            while (places >= 2) {
                number *= 100;
                int two = (int) number;
                number -= two;
                uint8_t lookup_value = lookup[two];
                write_char((lookup_value >> 4) + '0');
                write_char((lookup_value & 15) + '0');
                places -= 2;
            }
 
            if (places == 1) {
                number *= 10;
                int one = (int) number;
                write_char(one + '0');
            }
        }
 
        if (after)
            write_char(after);
    }
 
    void init() {
        // Make sure _flush_output() is called at the end of the program.
        bool exit_success = atexit(_flush_output) == 0;
        assert(exit_success);
 
        for (int i = 0; i < 100; i++)
            lookup[i] = (i / 10 << 4) + i % 10;
    }
}   
 
template <typename T> 
void print_vec(const vector<T>& v, bool newline = true) {
    for (int i = 0; i < v.size(); ++i) {
        cout << v[i] << " ";
    }
    if (newline) {
        cout << "\n";
    }
}
 
mt19937 rng((uint32_t)chrono::steady_clock::now().time_since_epoch().count()); 
 
const int maxn = (int)1e6 + 505;
const int mod = (int)1e9 + 7; 
 
#define sz(x) ((int)(x.size()))
 
#define pb push_back
#define rep(i,a,b) for (int i = (a); i <= (b); i++)
#define repr(i,a,b) for (int i = (a); i >= (b); i--)
 
int fact_setup=0; int * fact; 
 
int ex (int a, int b){
    if(b==0)return 1; int e = ex(a,b/2); e=(1ll*e*e)%mod; if(b&1)e=(1ll*e*a)%mod; return e;
}
int inv (int a){
    return ex(a, mod - 2); 
}
int choose (int a, int b){
    if(!fact_setup){
        fact_setup=1; 
        fact = new int [maxn]; 
        fact[0]=1; rep (i,1,maxn-1) fact[i]=(i*fact[i-1])%mod; 
    }
    if(a<b)return 0; 
    int num = fact[a]; 
    int denom = (fact[b] * fact[a - b]) % mod; 
    return (num * inv(denom)) % mod; 
}
 
#define pii pair <int, int> 
#define pvi pair <vector <int>, int> 
 
#define pai array <int,2>
#define pdi array <double, 2> 
#define pdt array <double, 3>
 
#define pti array <int,3>
#define pfi array <int,4>
 
#define all(v) v.begin(), v.end()
 
int pref_max[4*maxn];
int lz[4*maxn]; 
 
void build (int start, int end, int node){
	if (start == end){
		pref_max[node] = start; lz[node]=0; return; 
	}
	build(start,(start+end)/2,2*node); 
	build((start+end)/2+1,end,2*node+1); 
	pref_max[node]=min(pref_max[2*node],pref_max[2*node+1]); 
	lz[node]=0;  
}
void push (int start, int end, int node){
	if (lz[node]){
		pref_max[node]+=lz[node]; 
		if(start!=end){
			lz[2*node]+=lz[node]; lz[2*node+1]+=lz[node]; 
		}
		lz[node]=0; 
	}
}
void upd (int start, int end, int i, int j, int val, int node=1){
	push(start, end, node); 
	if (start>j||end<i) return; 
	if(start>=i && end<=j){
		lz[node]=val; push(start, end, node); 
		return; 
	}
	upd(start,(start+end)/2,i,j,val,2*node); 
	upd((start+end)/2+1,end,i,j,val,2*node+1); 
	pref_max[node]=min(pref_max[2*node],pref_max[2*node+1]); 
}
 
int a [maxn], b[maxn]; 
vector<int>a_pos[2*maxn];
int b_pos[2*maxn]; 
int tot[2*maxn];
 
void upd (int x,int v,int n){
    for (int i=x; i<=n; i+=i&(-i)) tot[i]+=v; 
}
int query (int x){
    int ans = 0; 
    for (int i=x; i>0; i-=i&(-i))ans+=tot[i];
    return ans; 
}
 
main(){
    IO::init();
		
    int tc;
		IO::read_int(tc);
    rep (tt, 1, tc){
    	int n, m;
			IO::read_int(n);
			IO::read_int(m);
    	rep (i,1,n) IO::read_int(a[i]);
			rep (i,1,m) IO::read_int(b[i]);
    	build(0, n, 1); 
    	
    	map<int,int>is, comp;
    	rep (i,1,n) is[a[i]]=1; rep (i,1,m) is[b[i]]=1; 
    	int cnt=1; 
    	for (auto i=is.begin();i!=is.end();i++) {comp[i->first]=cnt++; }
    	cnt--; 
    	
    	rep (i,1,n)a[i]=comp[a[i]]; 
    	rep (i,1,m)b[i]=comp[b[i]]; 
    	rep (i,1,n)a_pos[a[i]].pb(i); 
    	rep (i,1,m)b_pos[b[i]]++;
    	
    	int la = 0; 
    	long long inv=0; 
    	rep (i,1,cnt){
    	    if(b_pos[i]==0){
    	       for (int j : a_pos[i]){
    			   upd(0, n, 0, n, 1); 
    	           upd(0, n, j, n, -2);
    		    }
    	        continue; 
    	    }
    		for (int j : a_pos[i]){
    			upd (0, n, j, n, -1); 
    		}
    		inv+=1LL*pref_max[1]*b_pos[i];  
    		for (int j : a_pos[i]){
    			upd (0, n, 0, j-1, 1); 
    		}
    	}
    	
    	rep (i,0,cnt){
    		a_pos[i].clear();b_pos[i]=0;
    	}
    	repr (i,n, 1){
    		inv += query(a[i]-1); 
    		upd(a[i], 1, cnt); 
    	}
    	rep (i,0,cnt) tot[i]=0; 
			IO::write_int(inv, '\n');
    }
    return 0; 
}
