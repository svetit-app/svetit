import { ComponentFixture, TestBed, waitForAsync } from '@angular/core/testing';

import { SaveTimersComponent } from './save-timers.component';

describe('SaveTimersComponent', () => {
  let component: SaveTimersComponent;
  let fixture: ComponentFixture<SaveTimersComponent>;

  beforeEach(waitForAsync(() => {
    TestBed.configureTestingModule({
      declarations: [ SaveTimersComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(SaveTimersComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
