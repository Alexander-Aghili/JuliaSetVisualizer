from manim import *
import math
import numpy as np
import sympy as sp

class ComplexProject(Scene):
    
    def construct(self):
        real_axis = NumberLine(x_range=[-5, 5, 1], length=10, include_tip=True)
        
        # Add integer labels to the real axis
        integer_labels = [str(i) for i in range(-5, 6)]
        integer_label_mobs = [Text(label, font_size=24).next_to(real_axis.number_to_point(int(label)), DOWN) for label in integer_labels]

        # Animate real axis with integer labels
        self.play(Create(real_axis))
        self.play(*[Write(label) for label in integer_label_mobs])
        self.wait(15)

        # Create complex plane
        complex_plane = Axes(x_range=[-5, 5, 1], y_range=[-5, 5, 1], 
                                    x_length=10, y_length=10, axis_config={"include_tip": True})

        # Transform real axis to complex plane
        self.play(ReplacementTransform(real_axis, complex_plane))
        self.wait(1)

        # Add labels for real and imaginary axes
        real_axis_label = Text("Re", font_size=24).next_to(complex_plane.c2p(5, 0), RIGHT)
        imag_axis_label = Text("Im", font_size=24).next_to(complex_plane.c2p(0, 5), UP)
        label_i = Text("i", font_size=24).next_to(complex_plane.c2p(0, 1), LEFT)
        label_minus_i = Text("-i", font_size=24).next_to(complex_plane.c2p(0, -1), RIGHT)
        label_0 = Text("0", font_size=24).next_to(complex_plane.c2p(0, 0), DOWN)

        # Animate the axis labels
        self.play(Write(real_axis_label), Write(imag_axis_label))
        self.play(Write(label_i), Write(label_minus_i), Write(label_0))
        self.wait(1)

        # Create ball
        ball_location = complex_plane.c2p(1, 1)
        ball = Dot(ball_location, color=BLUE)

        self.wait(2)
