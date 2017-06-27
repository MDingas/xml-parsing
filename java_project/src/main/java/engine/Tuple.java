package engine;

/**
*
* Generic object tuple
*
*/

public class Tuple<X, Y> {

    /* Instance variables */

    private X x;
    private Y y;

    /* Constructors */

    public Tuple(X x, Y y) {
        this.x = x;
        this.y = y;
    }

    public Tuple(Tuple tup){
            this((X)tup.x,(Y)tup.y);
    }

    /* Getters */

    public X getX(){ return this.x; }
    public Y getY(){ return this.y; }


    /* Setters */

    public void setX(X xarg){ this.x = xarg; }
    public void setY(Y yarg){ this.y = yarg; }

    public Tuple clone(){
            return new Tuple(this);
    }


}
