from manim import *
import math
import numpy as np
import sympy as sp

class ComplexProject(Scene):
    
    def construct(self):
 
        # Summary
        summary = VGroup(
            MathTex("bi"),
            MathTex("i = \\sqrt{-1}"),
        ).arrange(DOWN, center=True, aligned_edge=LEFT)

        self.wait(1)
        self.play(Write(summary))

        self.wait(20)
        self.play(FadeOut(summary))


       # Title

                # Creating the complex plane
        complex_plane = ComplexPlane(x_range=[-6, 6, 1], y_range=[-6, 6, 1],y_length=7,background_line_style={"stroke_opacity": 0.1})
        complex_plane.add_coordinates()
        self.play(Create(complex_plane))

        # Adding labels for axes
        real_label = complex_plane.get_axis_labels(x_label="Real Axis", y_label="Imaginary Axis")
        self.play(Write(real_label))

        self.wait(20)

        # Example 1: Complex number 3 + 4i
        point1 = Dot(complex_plane.n2p(3 + 4j), color=BLUE)
        label1 = MathTex("3 + 4i", font_size=24).next_to(point1, RIGHT)
        self.play(FadeIn(point1), Write(label1))

        # Drawing lines from origin to the point
        line1 = Line(complex_plane.n2p(0), complex_plane.n2p(3 + 4j), color=BLUE)
        self.play(Create(line1))
        self.wait(10)
        # Example 2: Complex number 5 - 2i
        point2 = Dot(complex_plane.n2p(5 - 2j), color=GREEN)
        label2 = MathTex("5 - 2i", font_size=24).next_to(point2, RIGHT)
        self.play(FadeIn(point2), Write(label2))
        self.wait(1)
        # Drawing lines from origin to the point
        line2 = Line(complex_plane.n2p(0), complex_plane.n2p(5 - 2j), color=GREEN)
        self.play(Create(line2))
        self.wait(5)

        # Displaying magnitude
        magnitude1 = MathTex("\\sqrt{3^2 + 4^2} = 5", font_size=24).next_to(line1, UP)
        self.play(Write(magnitude1))
        self.wait(2)
        self.play(FadeOut(magnitude1))

        # Displaying argument
                # Displaying magnitude
        magnitude2 = MathTex("\\sqrt{5^2 + (-2)^2} = \\sqrt{29}", font_size=24).next_to(line2, UP)
        self.play(Write(magnitude2))
        self.wait(2)
        self.play(FadeOut(magnitude2))

        angle1 = Angle(complex_plane.get_axes()[0], line1, radius=0.5, other_angle=False)
        theta1 = MathTex("\\theta = \\tan^{-1}\\left(\\frac{4}{3}\\right)", font_size=24).next_to(angle1, LEFT)
        self.play(Create(angle1), Write(theta1))
        self.wait(2)
        self.play(FadeOut(angle1), FadeOut(theta1))


        # Final complex plane fade out
        self.play(FadeOut(complex_plane), FadeOut(point2), FadeOut(label2), FadeOut(line2), FadeOut(point1), FadeOut(label1), FadeOut(line1), FadeOut(real_label))
        self.wait(1)

        # Summary
        summary = VGroup(
            MathTex("z = a + bi"),
            MathTex("a \\text{ is the real part}"),
            MathTex("b \\text{ is the imaginary part}"),
            MathTex("\\text{Magnitude: } |z| = \\sqrt{a^2 + b^2}"),
            MathTex("\\text{Argument: } \\theta = \\tan^{-1}\\left(\\frac{b}{a}\\right)")
        ).arrange(DOWN, center=True, aligned_edge=LEFT)

        self.wait(1)
        self.play(Write(summary))
        self.wait(1)
        self.play(FadeOut(summary))

        summary = VGroup(
                MathTex("f : \\mathbb{C} \\rightarrow \\mathbb{C} "),
        ).arrange(DOWN, center=True, aligned_edge=LEFT)

        self.wait(1)
        self.play(Write(summary))
        self.wait(10)
        self.play(FadeOut(summary))

        summary = VGroup(
                MathTex("f : \\mathbb{C} \\rightarrow \\mathbb{C} "),
                MathTex("f(z) = f(x+iy) = u(x,y) + i v(x,y)"),
                MathTex("x,y \\in \\mathbb{R} ")
        ).arrange(DOWN, center=True, aligned_edge=LEFT)

        self.wait(1)
        self.play(Write(summary))
        self.wait(10)




