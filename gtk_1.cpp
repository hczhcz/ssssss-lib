// FLTK is horrible // for lazy and stupid student such as HCZ
// So...
// Use GTK instead

#include <gtkmm.h>
#include <vector>

// Draw lines in an area
class Lines: public Gtk::DrawingArea
{
protected:
    float m_width;
    float m_r, m_g, m_b;

    typedef std::pair<guint, guint> t_point;
    typedef std::pair<t_point, t_point> t_data;
    typedef std::vector<t_data> t_data_list;
    t_data_list m_data;

    virtual bool on_draw(const Cairo::RefPtr<Cairo::Context> &cr)
    {
        cr->set_line_width(m_width);
        cr->set_source_rgb(m_r, m_g, m_b);

        // Iterate the list
        for (
            t_data_list::iterator iter = m_data.begin();
            iter != m_data.end();
            ++iter
        )
        {
            cr->move_to(iter->first.first, iter->first.second);
            cr->line_to(iter->second.first, iter->second.second);
        }

        cr->stroke();
        return true;
    }
public:
    Lines(float width = 1.0, float r = 0.0, float g = 0.0, float b = 0.0):
        m_width(width), m_r(r), m_g(g), m_b(b) {}

    void add_line(const guint x1, const guint y1, const guint x2, const guint y2)
    {
        m_data.push_back(t_data(t_point(x1, y1), t_point(x2, y2)));
    }

    void add_line(const Gtk::Widget &w1, const Gtk::Widget &w2)
    {

    }
};

// Text with a frame
class FramedText: public Gtk::Frame
{
protected:
    Gtk::Label m_label;
public:
    FramedText(const Glib::ustring &label):
        Gtk::Frame(), m_label(label)
        {
            add(m_label);
        }

    FramedText(const Glib::ustring &title, const Glib::ustring &label):
        Gtk::Frame(title), m_label(label)
        {
            add(m_label);
        }

    Gtk::Label &content() {return m_label;}
};

// Text with a title and a frame
class TitledText: public Gtk::VBox
{
protected:
    Gtk::Label m_title;
    FramedText m_frame;
public:
    TitledText(const Glib::ustring &title, const Glib::ustring &label):
        Gtk::VBox(), m_title(title), m_frame(label)
        {
            m_title.set_halign(Gtk::ALIGN_START);
            add(m_title);
            add(m_frame);
        }

    FramedText &content() {return m_frame;}
};

// Put a frame as a shadow
template <class T>
class ShadowFrame: public Gtk::Fixed
{
protected:
    guint m_fulloffset;

    Gtk::Frame m_frame;
    T &m_content;

    virtual bool on_draw(const Cairo::RefPtr<Cairo::Context> &cr)
    {
        SyncSize();
        return Gtk::Fixed::on_draw(cr);
    }
public:
    ShadowFrame(T &content, guint layer = 1, guint offset = 2):
        Gtk::Fixed(), m_fulloffset(layer * offset), m_frame(), m_content(content)
        {
            add(m_frame);

            m_content.set_margin_left(offset);
            m_content.set_margin_top(offset);
            add(m_content);
        }

    void SyncSize()
    {
        m_frame.set_size_request(
            m_content.get_allocated_width() - m_fulloffset,
            m_content.get_allocated_height() - m_fulloffset
        );
    }

    T &content() {return m_content;}
};

// Main
int main(int argc, char *argv[])
{
    // Init GTK
    Gtk::Main kit(argc, argv);
    Gtk::Window window;
    window.set_size_request(450, 350);

        Gtk::Fixed fixed;
        window.add(fixed);

            // Lines ////////

            Lines lines;
            lines.set_size_request(500, 425);

            // FLTK
            lines.add_line(230, 45, 110, 115);
            lines.add_line(270, 45, 270, 90);
            lines.add_line(290, 45, 400, 165);
            lines.add_line(310, 40, 350, 60);

            // Point
            lines.add_line(110, 55, 110, 115);
            lines.add_line(150, 55, 210, 90);

            // Graph
            lines.add_line(110, 160, 90, 240);
            lines.add_line(110, 160, 170, 340);

            // Window
            lines.add_line(250, 135, 230, 190);
            lines.add_line(270, 135, 270, 265);
            lines.add_line(320, 135, 400, 165);

            // GUI
            lines.add_line(340, 210, 270, 265);
            lines.add_line(390, 210, 400, 240);

            // Simple
            lines.add_line(200, 310, 170, 340);

            // So difficult... >.<

            fixed.add(lines);

            // Nodes ////////

            TitledText tt1("Point.h", "struct Point { ... };");
            fixed.put(tt1, 50, 25);

            TitledText tt2("Graph.h", "// graphing interface\nstruct Shape { ... };\n...");
            fixed.put(tt2, 50, 100);

            TitledText tt3("Graph.cpp", "Graph code");
            fixed.put(tt3, 25, 225);

            TitledText tt4("Window.h", "// window interface\nclass Window { ... };\n...");
            fixed.put(tt4, 200, 75);

            TitledText tt5("Window.cpp", "Window code");
            fixed.put(tt5, 150, 175);

            TitledText tt6("GUI.h", "// GUI interface\nstruct In_box { ... };\n...");
            fixed.put(tt6, 325, 150);

            TitledText tt7("GUI.cpp", "GUI code");
            fixed.put(tt7, 350, 225);

            TitledText tt8("Simple_window.h", "// window interface\nclass Simple_window { ... };\n...");
            fixed.put(tt8, 175, 250);

            TitledText tt9("chapter12.cpp", "#include \"Graph.h\"\n#include \"Simple_window.h\"\nint main { ... }");
            fixed.put(tt9, 75, 325);

            FramedText ft1("FLTK headers");
            ShadowFrame<FramedText> sfa1(ft1);
            ShadowFrame<ShadowFrame<FramedText> > sfb1(sfa1, 2);
            fixed.put(sfb1, 225, 25);

            FramedText ft2("FLTK code");
            ShadowFrame<FramedText> sfa2(ft2);
            ShadowFrame<ShadowFrame<FramedText> > sfb2(sfa2, 2);
            fixed.put(sfb2, 350, 50);

    // Done, run the application
    window.show_all_children();
    Gtk::Main::run(window);

    return EXIT_SUCCESS;
}
