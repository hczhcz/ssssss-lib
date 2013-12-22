#define _USE_MATH_DEFINES
#include <cmath>
#include <string>
#include <vector>
#include <gtkmm.h>

// Binary tree node ID
// Abstract head: 0
// Level 1 (Root): 1
// Level 2: 1->2 1->3
// Level 3: 2->4 2->5 3->6 3->7
// ...
typedef size_t node_id;

// Abstract drawing module for line
class DM
{
public:
    bool manage_by_widget;

    DM(): manage_by_widget(true) {};

    // Draw line from point 1 to point 2
    virtual void draw(
        const Cairo::RefPtr<Cairo::Context> &cr,
        guint x1, guint y1, guint x2, guint y2
    ) = 0;

    // Draw by allocation (cut the overlapped part)
    void draw(
        const Cairo::RefPtr<Cairo::Context> &cr,
        const Gtk::Allocation &a1, const Gtk::Allocation &a2
    )
    {
        // Middle points
        float x1 = a1.get_x() + a1.get_width() * 0.5;
        float y1 = a1.get_y() + a1.get_height() * 0.5;
        float x2 = a2.get_x() + a2.get_width() * 0.5;
        float y2 = a2.get_y() + a2.get_height() * 0.5;

        const float x = x2 - x1;
        const float y = y2 - y1;

        // If is not the same position, draw line
        if (x != 0 || y != 0)
        {
            float delta;

            // Cut from point 1
            if (a1.get_width() * abs(y) < a1.get_height() * abs(x))
            {
                delta = a1.get_width() * 0.5 / abs(x);
            }
            else
            {
                delta = a1.get_height() * 0.5 / abs(y);
            }
            x1 += delta * x;
            y1 += delta * y;

            // Cut from point 2
            if (a2.get_width() * abs(y) < a2.get_height() * abs(x))
            {
                delta = a2.get_width() * 0.5 / abs(x);
            }
            else
            {
                delta = a2.get_height() * 0.5 / abs(y);
            }
            x2 -= delta * x;
            y2 -= delta * y;

            // Draw it
            draw(cr, x1, y1, x2, y2);
        }
    }

    // Draw to link two widgets
    void draw(
        const Cairo::RefPtr<Cairo::Context> &cr,
        const Gtk::Widget &w1, const Gtk::Widget &w2
    )
    {
        draw(cr, w1.get_allocation(), w2.get_allocation());
    }
};

// Simple line drawing module
// As an example
class DMSimple: public DM
{
private:
    float m_width, m_r, m_g, m_b;
public:
    DMSimple(const float width = 1, const float r = 0, const float g = 0, const float b = 0):
        DM(), m_width(width), m_r(r), m_g(g), m_b(b) {}

    virtual void draw(
        const Cairo::RefPtr<Cairo::Context> &cr,
        guint x1, guint y1, guint x2, guint y2
    )
    {
        // Setup
        cr->set_line_width(m_width);
        cr->set_source_rgb(m_r, m_g, m_b);

        // Draw
        cr->move_to(x1, y1);
        cr->line_to(x2, y2);

        // Finish
        cr->stroke();
    }
};

// Factory of drawing modules
class DMF
{
public:
    virtual DM *create(const node_id id) = 0;
};

// Create a simple line drawing module
// Singleton
class DMFSimple: public DMF
{
private:
    DMSimple *m_obj;
public:
    DMFSimple():
        DMF(), m_obj(nullptr) {}

    ~DMFSimple()
    {
        if (m_obj)
        {
            delete m_obj;
        }
    }

    virtual DM *create(const node_id id)
    {
        if (!m_obj)
        {
            m_obj = new DMSimple();
            m_obj->manage_by_widget = false;
        }
        return m_obj;
    }
};

// Factory of binary tree nodes
class BTNodeF
{
public:
    virtual Gtk::Widget *create(const node_id id) = 0;
};

// Create a label (node ID) as node
class BTNodeFLabel: public BTNodeF
{
public:
    virtual Gtk::Widget *create(const node_id id)
    {
        return new Gtk::Label(std::to_string(id));
    }
};

// Position controller of binary tree
class BTPos
{
protected:
    // Count bit length of id
    // Get node's level
    inline guint bitlevel(node_id id)
    {
        guint result = 0;
        for (; id != 0; id >>= 1, ++result) {}

        return result;
    }
public:
    virtual void set_pos(const node_id id, const node_id max, Gtk::Widget &node, const guint width, const guint height) = 0;
};

// Position controller (tree style)
class BTPosClassic: public BTPos
{
public:
    virtual void set_pos(const node_id id, const node_id max, Gtk::Widget &node, const guint width, const guint height)
    {
        const guint levelid = bitlevel(id) - 1;

        const guint x = width * (int(id) - int(1 << levelid) + 1) / ((1 << levelid) + 1);
        const guint y = height * ((levelid << 1) + 1) / (bitlevel(max) << 1);

        node.size_allocate(
            Gtk::Allocation(
                x - node.get_allocated_width() / 2,
                y - node.get_allocated_height() / 2,
                node.get_allocated_width(),
                node.get_allocated_height()
            )
        );
    }
};

// Position controller (concentric style)
class BTPosConcentric: public BTPos
{
public:
    virtual void set_pos(const node_id id, const node_id max, Gtk::Widget &node, const guint width, const guint height)
    {
        const guint minsize = (width < height) ? width : height;
        const guint levelid = bitlevel(id) - 1;
        const guint levelmax = bitlevel(max) - 1;

        // Radius
        const guint r = minsize * levelid / (levelmax * 2 + 1);

        // Angle
        float a = M_PI * float(int(id << 1) - int(2 << levelid) + 1) / float(1 << levelid);
        // float a = M_PI / 16 * id;

        // Position
        const guint x = width / 2 - round(r * sin(a));
        const guint y = height / 2 - round(r * cos(a));

        node.size_allocate(
            Gtk::Allocation(
                x - node.get_allocated_width() / 2,
                y - node.get_allocated_height() / 2,
                node.get_allocated_width(),
                node.get_allocated_height()
            )
        );
    }
};

// Position controller (spiral style)
class BTPosSpiral: public BTPos
{
public:
    virtual void set_pos(const node_id id, const node_id max, Gtk::Widget &node, const guint width, const guint height)
    {
        const guint minsize = (width < height) ? width : height;
        const guint levelid = bitlevel(id) - 1;
        const guint levelmax = bitlevel(max) - 1;

        // Radius
        const float r = minsize * log(id) / (log(max) * 2 + 0.5);

        // Angle
        float a = M_PI * float(int(id << 1) - int(2 << levelid) + 1) / float(1 << levelid);
        // float a = M_PI / 16 * id;

        // Position
        const guint x = width / 2 - round(r * sin(a));
        const guint y = height / 2 - round(r * cos(a));

        node.size_allocate(
            Gtk::Allocation(
                x - node.get_allocated_width() / 2,
                y - node.get_allocated_height() / 2,
                node.get_allocated_width(),
                node.get_allocated_height()
            )
        );
    }
};

// Target of callback canvas
class CanvasTarget
{
public:
    virtual bool do_draw(const Cairo::RefPtr<Cairo::Context> &cr) = 0;
};

// Callback canvas
class Canvas: public Gtk::DrawingArea
{
private:
    CanvasTarget &m_target;
protected:
    virtual bool on_draw(const Cairo::RefPtr<Cairo::Context> &cr)
    {
        return m_target.do_draw(cr);
    }
public:
    Canvas(CanvasTarget &target): Gtk::DrawingArea(), m_target(target) {}
};

// Binary tree widget
// Gtk::Fixed => canvas
// std::vector => nodes & lines
class BTView: public Gtk::Fixed, public CanvasTarget, public std::vector<std::pair<Gtk::Widget *, DM *> >
{
private:
    BTPos &m_pos;
    Canvas m_canvas;
protected:
    // Draw
    virtual bool on_draw(const Cairo::RefPtr<Cairo::Context> &cr) {
        Gtk::Fixed::on_draw(cr);
        return true;
    }

    // Set nodes' position and draw lines
    virtual bool do_draw(const Cairo::RefPtr<Cairo::Context> &cr)
    {
        // Set position
        {
            // Skip abstract head (id = 0)
            node_id iter1 = 1;

            // Set each node's position
            for (; iter1 < size(); ++iter1)
            {
                if (can_get_widget(iter1))
                {
                    m_pos.set_pos(iter1, size() - 1, get_widget(iter1), get_allocated_width(), get_allocated_height());
                }
            }
        }

        // Sync size
        m_canvas.size_allocate(
            Gtk::Allocation(
                0, 0, get_allocated_width(), get_allocated_height()
            )
        );

        // Draw lines
        {
            // Skip abstract head (id = 0) and level 1 (root, id = 1)
            node_id iter2 = 2;

            // Draw each line
            for (; iter2 < size(); ++iter2)
            {
                if (can_get_dm(iter2))
                {
                    get_dm(iter2).draw(cr, get_widget(iter2 >> 1), get_widget(iter2));
                }
            }

            return true;
        }
    }
public:
    BTView(BTNodeF &nodes, DMF &lines, BTPos &pos, const guint level):
        Gtk::Fixed(), std::vector<std::pair<Gtk::Widget *, DM *> >(1 << level),
        m_pos(pos), m_canvas(*this)
    {
        add(m_canvas);

        // Construct nodes
        // Skip abstract head (id = 0)
        for (node_id iter = 1; iter < size(); ++iter)
        {
            // Check if is root or parent exist
            if (iter == 1 || can_get_widget(iter >> 1))
            {
                Gtk::Widget *node = nodes.create(iter);
                if (node != nullptr)
                {
                    (*this)[iter].first = node;
                    add(*node);
                }
            }
        }

        // Construct lines
        // Skip abstract head (id = 0) and level 1 (root, id = 1)
        for (node_id iter = 2; iter < size(); ++iter)
        {
            // Check if connectable
            if (can_get_widget(iter >> 1) && can_get_widget(iter))
            {
                (*this)[iter].second = lines.create(iter);
            }
        }
    }

    virtual ~BTView()
    {
        // Free nodes and lines
        for (auto iter = begin(); iter != end(); ++iter)
        {
            if (iter->first != nullptr)
            {
                delete iter->first;
            }
            if (iter->second != nullptr && iter->second->manage_by_widget)
            {
                delete iter->second;
            }
        }
    }

    Gtk::Widget &get_widget(const node_id index)
    {
        return *((*this)[index].first);
    }

    DM &get_dm(const node_id index)
    {
        return *((*this)[index].second);
    }

    bool can_get_widget(const node_id index)
    {
        return (*this)[index].first != nullptr;
    }

    bool can_get_dm(const node_id index)
    {
        return (*this)[index].second != nullptr;
    }
};

// Example of custom nodes
class BTNodeFLabel1: public BTNodeFLabel
{
public:
    virtual Gtk::Widget *create(const node_id id)
    {
        if (id == 5)
        {
            // Change the node
            return new Gtk::Button("(o.O)");
        }
        if (id == 11)
        {
            // Cut off the tree
            return nullptr;
        }
        else
        {
            return BTNodeFLabel::create(id);
        }
    }
};

// Example of custom lines
class DMFSimple1: public DMFSimple
{
public:
    virtual DM *create(const node_id id)
    {
        if (id < 40)
        {
            // Change line style
            return new DMSimple(2 - id * 0.025, 0, 0, 1 - id * 0.025);
        }
        else
        {
            return DMFSimple::create(id);
        }
    }
};

// Main
int main(int argc, char *argv[])
{
    // Init GTK
    Gtk::Main kit(argc, argv);
    Gtk::Window window;
    window.set_size_request(560, 420);

    // Create view
    BTNodeFLabel1 n;
    DMFSimple1 d;
    // BTPosClassic p;
    BTPosConcentric p;
    // BTPosSpiral p;
    BTView bt(n, d, p, 6);
    window.add(bt);

    // Done, run the application
    window.show_all_children();
    Gtk::Main::run(window);

    return EXIT_SUCCESS;
}
