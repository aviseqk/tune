package components

import (
	"fmt"
	"github.com/charmbracelet/lipgloss"
)

var fieldsWidth struct {
	pid, name, command, owner, threads, mem, cpu int
}
var headerString string

type ProcessBox struct {
	width       int
	height      int
	Pid         int
	ProgramName string
	Threads     int
	Command     string
	Owner       string
	MemUsage    float64
	CpuUsage    float64
	Expanded    bool // this field is for the state that makes children be rendered on the tui
	Depth       int
	Children    []*ProcessBox
}

func (p *ProcessBox) SetSize(w, h int) {
	p.width = w
	p.height = h

	p.calculateWidths()
}

func (p *ProcessBox) NewProcessBox() *ProcessBox {
	process := ProcessBox{}

	return &process
}

func flatten(nodes []*ProcessBox) []*ProcessBox {
	var out []*ProcessBox
	var walk func([]*ProcessBox, int)
	walk = func(procs []*ProcessBox, depth int) {
		for _, proc := range procs {
			proc.Depth = depth
			out = append(out, proc)
			if proc.Expanded {
				walk(proc.Children, depth+1)
			}
		}
	}
	walk(nodes, 0)
	return out
}

func (p *ProcessBox) calculateWidths() {
	windowWidth := p.width
	width1 := windowWidth * 2 / 5
	fieldsWidth.command = width1 * 3 / 5
	fieldsWidth.name = width1 * 2 / 5

	width2 := (p.width - width1) / 2
	fieldsWidth.pid = width2 / 2
	fieldsWidth.owner = width2 / 2

	width3 := p.width - width1 - width2
	fieldsWidth.threads = width3 / 3
	fieldsWidth.cpu = width3 / 3
	fieldsWidth.mem = width3 / 3

}

func RenderHeader() string {

	baseStyle := lipgloss.NewStyle().Align(lipgloss.Right)

	pidStr := baseStyle.Width(fieldsWidth.pid).Render("Pid:")
	nameStr := baseStyle.Width(fieldsWidth.name).Render("Program:")
	commandStr := baseStyle.Width(fieldsWidth.command).Render("Command:")
	ownerStr := baseStyle.Width(fieldsWidth.owner).Render("Owner:")
	threadsStr := baseStyle.Width(fieldsWidth.threads).Render("#Thr:")
	cpuStr := baseStyle.Width(fieldsWidth.cpu).Render("CpuUsage:")
	memStr := baseStyle.Width(fieldsWidth.mem).Render("MemUsage:")

	return lipgloss.JoinHorizontal(lipgloss.Top, pidStr, nameStr, commandStr, threadsStr, ownerStr, cpuStr, memStr)
}

func (p *ProcessBox) View() string {
	//var usageStr strings.Builder
	name := p.ProgramName
	if len(p.Children) > 0 {
		prefix := ">"
		if p.Expanded {
			prefix = "V"
		}
		name = prefix + " " + name
	}
	baseStyle := lipgloss.NewStyle().Align(lipgloss.Right)

	pidStr := baseStyle.Width(fieldsWidth.pid).Render(fmt.Sprint(p.Pid))
	nameStr := baseStyle.Foreground(lipgloss.Color("#7DCA98")).Width(fieldsWidth.name).Render(fmt.Sprint(name))
	commandStr := baseStyle.Width(fieldsWidth.command).Render(fmt.Sprint(p.Command))
	ownerStr := baseStyle.Foreground(lipgloss.Color("#7DCA98")).Width(fieldsWidth.owner).Render(fmt.Sprint(p.Owner))
	threadsStr := baseStyle.Width(fieldsWidth.threads).Render(fmt.Sprint(p.Threads))
	cpuStr := baseStyle.Width(fieldsWidth.cpu).Render(fmt.Sprintf("%.1f%%", p.CpuUsage))
	memStr := baseStyle.Width(fieldsWidth.mem).Render(fmt.Sprintf("%.1f%%", p.MemUsage))

	//line := fmt.Sprintf("%s %d %-10s %-10s %-10s %.2f%% %.2f%%\n", sel, p.Pid, name, p.Command, p.Owner, p.CpuUsage, p.MemUsage)

	return lipgloss.JoinHorizontal(lipgloss.Top, pidStr, nameStr, commandStr, threadsStr, ownerStr, cpuStr, memStr)

	//usageStr.WriteString(sel)
	//usageStr.WriteString(line)

	//return lipgloss.NewStyle().Width(p.width).Render(usageStr.String())
}
