/* project package */
package engine;

/* xml parser */
import javax.xml.stream.XMLStreamConstants;
import javax.xml.stream.XMLStreamException;
import javax.xml.stream.XMLInputFactory;
import javax.xml.stream.XMLStreamReader;

/* io */
import java.io.FileNotFoundException;
import java.io.UnsupportedEncodingException;
import java.io.FileInputStream;

/* data sets */
import java.util.Collections.*;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/* streams */
import java.util.stream.Collectors;

/* method interface */
import li3.Interface;

/**
 * Class that is responsible for loading xml files and
 * answering a predefined number of queries
 *
 */

public class QueryEngineImpl implements Interface {
    /* Instance variables */

    private long totalArticles; /* Total articles viewed */
    private long totalRevisions; /* Total number of unique id and revision id pairs viewed */
    private long uniqueArticles; /* Total number of unique article ids viewed */
    Map<String,Article> articletable; /* Map an article's id to the article itself */
    Map<String,Contributor> usertable; /* Map a user's id to the user itself */

    public void init() {
        this.totalArticles = 0;
        this.totalRevisions = 0;
        this.uniqueArticles = 0;
        this.articletable = new HashMap<String,Article>();
        this.usertable = new HashMap<String,Contributor>();
    }

    public void load(int nsnaps, ArrayList<String> snaps_paths) {
        /* Algorithm:
         * fill an article and contributor node with all the info i find in the current page;
         * in the end of each loop, i check if those already exist in the hashtables and update accordingly
         */

        for(int i = 0; i < nsnaps; i++){
            String file = snaps_paths.get(i); /* current snapshot to look at */
            XMLInputFactory xmlInputFactory = XMLInputFactory.newInstance();

            /* Search varibles */
            /* Article search variables */
            String id = "n/a";
            String title = "n/a";
            String revid = "n/a";
            String timestamp = "n/a";
            Integer textSize = -1;
            Integer textWords = -1;

            /* Contributor search variables */
            String username = "n/a";
            String userid = "n/a";

            boolean newrevision = false;


            try{
                XMLStreamReader xmlStreamReader = xmlInputFactory.createXMLStreamReader(new FileInputStream(file));

                while(true){

                    int event = xmlStreamReader.getEventType();
                    switch(event) {

                        case XMLStreamConstants.START_ELEMENT: /* Inicial part of the nodes, retrieve information here */
                            /* PAGE NODE */
                            if(xmlStreamReader.getLocalName().equals("page")){ /* We found a new page, inicialize article/contributor nodes */
                                this.totalArticles++;

                                /* TITLE NODE */
                            }else if(xmlStreamReader.getLocalName().equals("title")){
                                title = xmlStreamReader.getElementText();

                                /* ID NODES (article id, revision id, contributor id) */
                            }else if(xmlStreamReader.getLocalName().equals("id")){
                                if(id.equals("n/a")) /* First appears the id node...*/
                                    id = xmlStreamReader.getElementText();

                                else if(revid.equals("n/a")){ /* Then the revid node... */
                                    revid = xmlStreamReader.getElementText();

                                }else{ /* And finally the contributor id node */
                                    userid = xmlStreamReader.getElementText();
                                }

                                /* TIMESTAMP NODE */
                            }else if(xmlStreamReader.getLocalName().equals("timestamp")){
                                timestamp = xmlStreamReader.getElementText();

                                /* USERNAME NODE */
                            }else if(xmlStreamReader.getLocalName().equals("username")){
                                username = xmlStreamReader.getElementText();

                                /* TEXT NODE */
                            }else /* Only calculate text size if it is a truly new article */
                                if(xmlStreamReader.getLocalName().equals("text")
                                        && (!this.articletable.containsKey(id)
                                            || ( this.articletable.containsKey(id) && !this.articletable.get(id).containsRevid2(revid)))){
                                    String text = xmlStreamReader.getElementText();
                                    Tuple<Integer,Integer> answer = Article.bytes_words(text);
                                    textSize = (int)answer.getX();
                                    textWords = (int)answer.getY();
                                            }
                            break;

                            /* The page node ended, let's use all the information we gathered here */
                        case XMLStreamConstants.END_ELEMENT:
                            if(xmlStreamReader.getLocalName().equals("page")){
                                Tuple<String,String> pair = new Tuple(revid,timestamp);
                                if(!this.articletable.containsKey(id)){ /* Is it an entirely new article */
                                    Article currArticle = new Article(title,id,revid,textSize,textWords);
                                    currArticle.addNewRevPair(pair);

                                    newrevision = true;

                                    this.uniqueArticles++;
                                    this.totalRevisions++;

                                    this.articletable.put(currArticle.getId(),currArticle);
                                }else{
                                    Article existingArt = this.articletable.get(id);
                                    if(existingArt != null){ /* The article node already exists, update information */
                                        if(existingArt.getTextwords() < textWords){
                                            existingArt.setTextwords(textWords);
                                        }
                                        if(existingArt.getTextsize() < textSize){
                                            existingArt.setTextsize(textSize);
                                        }
                                        if(!existingArt.containsRevid(pair)){ /* Is it a new revision */
                                            newrevision = true;
                                            this.totalRevisions++;
                                            existingArt.addNewRevPair(pair);
                                        }
                                    }
                                }
                                if(newrevision){ /* We found a new revision, let's update the contributor's number of revisions */
                                    Contributor existingContributor = this.usertable.get(userid);
                                    if(existingContributor != null){  /* Is it an already new user? Update node */
                                        existingContributor.incEntries();
                                    }
                                    else{ /* Is it a new user? Create node */
                                        Contributor currContributor = new Contributor(username,userid);
                                        currContributor.incEntries();
                                        this.usertable.put(currContributor.getUserid(),currContributor);
                                    }
                                }

                                /* Reset every auxiliary variables every iteration */
                                id = "n/a";
                                title = "n/a";
                                revid = "n/a";
                                timestamp = "n/a";
                                textSize = -1;
                                textWords = -1;

                                username = "n/a";
                                userid = "n/a";

                                newrevision = false;
                            }
                            break;

                        default:
                            break;
                    }
                    if (!xmlStreamReader.hasNext()) /* Terminate loop if there are no more nodes to look at */
                        break;

                    event = xmlStreamReader.next();
                }
            } catch(FileNotFoundException | XMLStreamException e){ e.printStackTrace(); }
        }
    }

    public long all_articles() { return this.totalArticles; }

    public long unique_articles() { return this.uniqueArticles; }

    public long all_revisions() { return this.totalRevisions; }

    public ArrayList<Long> top_10_contributors() {
        int tableSize = this.usertable.size();
        return new ArrayList(
                this.usertable
                .values()
                .stream()
                .filter(c -> !c.getUserid().equals("n/a")) /* remove unidentified entries */
                .sorted() /* natural sort : by number of entries */
                .limit(tableSize >= 10 ? 10 : tableSize) /* Take first 10 if you can, else take everything */
                .map(Contributor :: getUserid)
                .map(Long 		 :: parseLong)
                .collect(Collectors.toList()));
    }

    public String contributor_name(long contributor_id) {
        Contributor contributor = this.usertable.get(Long.toString(contributor_id));
        return(contributor != null ? contributor.getUsername() : "not found");
    }

    public ArrayList<Long> top_20_largest_articles() {
        int tableSize = this.articletable.size();
        return new ArrayList(
                this.articletable
                .values()
                .parallelStream()
                .filter(a -> !a.getId().equals("n/a")) /* remove unidentified entries */
                .sorted(Article.Comparators.SIZE_DESCENDING)
                .limit(tableSize >= 20 ? 20 : tableSize) /* Take first 20 if you can , else take everything */
                .map(Article :: getId)
                .map(Long 	 :: parseLong)
                .collect(Collectors.toList()));
    }

    public String article_title(long article_id) {
        Article article = this.articletable.get(Long.toString(article_id));
        return(article != null ? article.getTitle() : "not found");
    }

    public ArrayList<Long> top_N_articles_with_more_words(int n) {
        int tableSize = this.articletable.size();
        return new ArrayList(
                this.articletable
                .values()
                .parallelStream()
                .filter(c -> !c.getId().equals("n/a")) /* remove unidentified entries */
                .sorted(Article.Comparators.WORDS_DESCENDING)
                .limit(tableSize >= n ? n : tableSize) /* Take first n if you can, else take everything */
                .map(Article :: getId)
                .map(Long 	 :: parseLong)
                .collect(Collectors.toList()));
    }

    public ArrayList<String> titles_with_prefix(String prefix) {
        return new ArrayList(
                this.articletable
                .values() /* get articles in a list */
                .parallelStream()
                .map(a -> a.getTitle())
                .filter(title -> title.startsWith(prefix))
                .sorted() /* alphabetical */
                .collect(Collectors.toList()));
    }

    public String article_timestamp(long article_id, long revision_id) {
        Article article = this.articletable.get(Long.toString(article_id));
        if(article == null)
            return "not found";
        List<Tuple<String,String>> list = article.getRevPair();
        for(Tuple<String,String> pair : list)
            if(pair.getX().equals(Long.toString(revision_id)))
                return (pair.getY());
        return "not found";
    }

    /* Java's virtual machine deletes objects which are null or not referenced by anything */
    public void clean() {
        this.articletable = null;
        this.usertable = null;
    }
}
