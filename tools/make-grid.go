package main

import (
	"image"
	"image/color"
	"image/png"
	"log"
	"math"
	"os"
)

// Parameters: can be adjusted
const size = 1000             // side length of output image
const cells = 10              // number of cells in grid along one side, must be a factor of size
const stroke = 2              // pixel width of line stroke, must be even
const outputFileName = "grid" // name of output file (without ext)
var strokeColor = black(0.25) // color of line stroke

/// black returns the color black with specified alpha
func black(alpha float64) color.Color {
	if alpha < 0 || alpha > 1 {
		panic("alpha must be in [0, 1]")
	}
	a8 := uint8(alpha * float64(math.MaxUint8))
	return &color.NRGBA{A: a8}
}

func main() {
	// assert constants
	if stroke < 0 || stroke%2 != 0 {
		log.Fatalln("stroke must be even and positive")
	}
	if size%cells != 0 {
		log.Fatalln("cell count must be a factor of image size")
	}

	// create image buffer
	img := image.NewNRGBA(image.Rect(0, 0, size, size))

	// draw grid
	const cellSize = size / cells
	for cell := 0; cell < (cells + 1); cell += 1 {
		// for every cell, draw two border lines
		for t := 0; t < size; t += 1 {
			// for every pixel `t` along the border line
			for s := -stroke / 2; s < stroke/2; s += 1 {
				// for every pixel `s` across the line stroke

				img.Set((cell*cellSize)+s, t, strokeColor) // top-down line
				img.Set(t, (cell*cellSize)+s, strokeColor) // left-right line
			}
		}
	}

	// create output file
	f, err := os.Create(outputFileName + ".png")
	if err != nil {
		log.Fatalln(err)
	}
	defer f.Close()

	// save image to png file
	err = png.Encode(f, img)
	if err != nil {
		log.Fatalln(err)
	}
	log.Println("saved to", outputFileName)
}
