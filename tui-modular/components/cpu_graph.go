package components

import (
	"fmt"

	"github.com/charmbracelet/lipgloss"
)

type GraphBox struct {
	usage         int
	width, height int
	inputs        []int
}

func (g *GraphBox) SetSize(w, h int) {
	g.width = w
	g.height = h
}

func (g *GraphBox) SetUsage(u int) {
	g.usage = u
	g.AddUsageForGraph(u)
}

func (g *GraphBox) AddUsageForGraph(u int) {
	g.inputs = append(g.inputs, u)
}

func (c GraphBox) View() string {
	// box := buildCpuGraphBox("CPU Graph: ", "CPU", c.usage)

	usageStr := "Graph Box\n"
	usageStr += fmt.Sprintf("%d", c.usage)

	boxStyle := lipgloss.NewStyle().Width(c.width).
		Height(c.height).
		Border(lipgloss.NormalBorder()).
		Align(lipgloss.Center).
		Padding(0, 1)

	return boxStyle.Render(usageStr)
}

func NewGraphBox() *GraphBox {
	// p := progress.New(progress.WithDefaultGradient())
	// p.ShowPercentage = false

	return &GraphBox{
		usage: 20,
	}
}

// func buildCpuGraphBox(title, label string, usage int) string {

// 	width := 150
// 	content := fmt.Sprintf("Usage: %d%%\n[████▁▁▁▁]", usage)

// 	// Create embedded title
// 	embed := fmt.Sprintf(" %s ", title)
// 	left := "╭"
// 	right := "╮"
// 	h := "─"

// 	remain := width - len(embed) // Subtract corner chars
// 	lpad := remain / 2
// 	rpad := remain - lpad

// 	top := left + strings.Repeat(h, lpad) + embed + strings.Repeat(h, rpad) + right

// 	// Middle content with vertical sides
// 	var mid strings.Builder
// 	lines := strings.Split(content, "\n")
// 	for _, line := range lines {
// 		mid.WriteString(fmt.Sprintf("│ %-*s │\n", width-2, line))
// 	}

// 	// Bottom border
// 	bottom := "╰" + strings.Repeat(h, width-2) + "╯"

// 	return top + "\n" + mid.String() + bottom
// }
