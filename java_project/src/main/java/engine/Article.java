package engine;

import java.util.List;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.stream.Collectors;

/**
*
* Gathered of all query-relevant information of a Wikipedia article
*
*/

public class Article{

	/* Instance variables */

	private String title;
	private String id;
	private String revid; /* revision id */
	private int textSize; /* Number of bytes in the article's text node */
	private int textWords; /*Number of words in the article's text node */
	private ArrayList<Tuple<String,String>> revidTmstp; /* List of revision id & timestamp pairs seen */

	/* Constructors */

	public Article(){
		this.title = "n/a";
		this.id = "n/a";
		this.textSize = 0;
		this.textWords = 0;
		this.revidTmstp = new ArrayList<Tuple<String,String>>();
	}

	public Article(String t,String id,String rvid,Integer ts,Integer tw){
		this.title = t;
		this.id = id;
		this.textSize = ts;
		this.textWords = tw;
		this.revidTmstp = new ArrayList<Tuple<String,String>>();
	}

    public Article(Article a){
            this(a.title,a.id,a.revid,a.textSize,a.textWords);
    }

	/* Getters */

	public String getTitle(){ return this.title; }
	public String getId(){ return this.id; }
	public Integer getTextsize(){ return this.textSize; }
	public Integer getTextwords(){ return this.textWords; }
	public ArrayList<Tuple<String,String>> getRevPair(){
        return
            new ArrayList(this.revidTmstp.stream()
                              .map(Tuple :: clone)
                              .collect(Collectors.toList()));
    }

	/* Setters */

	public void setId(String id){ this.id = id; }
	public void setTitle(String title){ this.title = title; }
	public void setTextsize(int n){ this.textSize = n; }
	public void setTextwords(int n){ this.textWords = n; }
	public void addNewRevPair(Tuple<String,String> pair){ this.revidTmstp.add(pair.clone()); }

	/* Auxiliary */

	public boolean containsRevid(Tuple<String,String> pair){
		for(Tuple<String,String> p : this.revidTmstp)
			if((p.getX().equals(pair.getX())))
				return true;
		return false;
	}

	public boolean containsRevid2(String revid){
		for(Tuple<String,String> p : this.revidTmstp)
			if((p.getX().equals(revid)))
				return true;
		return false;
	}

	public static boolean segmentChar(char c){
		return (c == ' ' || c == '\t' || c == '\n');
	}

    public static Tuple<Integer,Integer> bytes_words(String text){
		char[] string = text.toCharArray();
		int i = 0;
		int spaces = 0;
        int nbytes = 0;

        nbytes += byteWorth(string[i]);

		if(segmentChar(string[i]))
        nbytes -= byteWorth(string[i]);
			while(segmentChar(string[i])){
                nbytes += byteWorth(string[i]);
				i++;
            }

		for(;i < text.length() - 1; i++){
            nbytes += byteWorth(string[i]);
			if(segmentChar(string[i])){
                nbytes -= byteWorth(string[i]);
				spaces++;
				while(segmentChar(string[i]) && (i < text.length() - 1)){
                    nbytes += byteWorth(string[i]);
					i++;
                }
				i--;
			}
		}
        return new Tuple(nbytes,spaces);
    }

	public static int byteWorth(char c){
		if(c <= 0x7f ) return 1;
		if(c <= 0x7ff) return 2;
		if(c <= 0xffff) return 3;
		return 4;
	}

	public static class Comparators {

		public static Comparator<Article> SIZE_DESCENDING = new Comparator<Article>(){
			public int compare(Article a1,Article a2){
				if(a1.textSize < a2.textSize)
					return 1;
				if(a1.textSize > a2.textSize)
					return -1;
				return(a1.id.compareTo(a2.id));
			}
		};

		public static Comparator<Article> WORDS_DESCENDING = new Comparator<Article>(){
			public int compare(Article a1,Article a2){
				if(a1.textWords < a2.textWords)
					return 1;
				if(a1.textWords > a2.textWords)
					return -1;
				return(a1.id.compareTo(a2.id));
			}
		};
	};

}
