from manim import *

class JuliaSet(Scene):
    def construct(self):
        # Equation
        equation = MathTex("f(z) = z^2 + c", font_size=48)
        self.play(Write(equation))
        self.wait(13)
        self.play(FadeOut(equation))
        
        julia = VGroup(
                MathTex("J(f) = \\{\\lim_{n\\rightarrow \\infty} (z_{n+1} = f(z_n})) \\not = \\infty"),
                MathTex("\\lim_{n\\rightarrow \\infty} (z_{n+1} = f(z_n})) \\not = z_0 , z_0 \\in \\mathbb{C}\\}")
        ).arrange(DOWN, center=True, aligned_edge=LEFT).scale(0.8)
        self.play(Write(julia))
        self.wait(20)
        self.play(FadeOut(julia))
