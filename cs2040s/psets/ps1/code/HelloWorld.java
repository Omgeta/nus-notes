public class HelloWorld {
    static int MysteryFunction(int argA, int argB) {
        int c = 1;
        int d = argA;
        int e = argB;
        while (e > 0) {
            if (2 * (e / 2) != e) {
                c = c * d;
            }
            d = d * d;
            e = e / 2;
        }
        return c;
    }

    public static void main(String args[]) {
        System.out.println("Hi, my name is Anoop");
        System.out.println("My favourite algorithms are Delaunay triangulation algos (looks cool)");
        System.out.println("I'm learning French. I use neovim as my primary code editor. I wish I took Literature instead of CS.");
        int output = MysteryFunction(-2, 4);
        System.out.println("The answer is: " + output + ".");
        System.out.println("MysteryFunction calculates Math.pow(argA, argB) (argA raised to the power argB)");
    }
}