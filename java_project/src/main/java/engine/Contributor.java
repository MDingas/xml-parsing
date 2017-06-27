package engine;

import java.util.Comparator;

/**
*
* Gatherer of all query-relevant information of a Wikipedia contributor
*
*/

public class Contributor implements Comparator<Contributor>, Comparable<Contributor>{

    /* Instance varibles */

    private String username;
    private String userid;
    private long entries; /* Number of unique revisions the contributor made */

    /* Constructors */

    public Contributor(){
        this.username = "n/a";
        this.userid = "n/a";
        this.entries = 0L;
    }

    public Contributor(String username,String userid){
        this.username = username;
        this.userid = userid;
        this.entries = 0L;
    }

    public Contributor(Contributor c){
        this(c.username,c.userid);
    }

    /* Getters */

    public String getUsername(){ return this.username; }
    public String getUserid(){ return this.userid; }
    public long getEntries(){ return this.entries; }

    /* Setters */

    public void setUsername(String name){ this.username = name; }
    public void setUserid(String id){ this.userid = id; }
    public void incEntries(){ this.entries++; }

    /* Methods */

    @Override
    public Contributor clone(){
        return new Contributor(this);
    }


    /* Sorting by entries in DESCENDING order */
    @Override
    public int compareTo(Contributor c){
        return ((-1)*(Long.compare(this.entries,c.entries)));
    }

    @Override
    public int compare(Contributor c1, Contributor c2){

        return (c1.compareTo(c2));
    }
}
